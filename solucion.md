# Solución — Simulador de Ferrys Margarita
**TAO207 · Sección 08 · Ismael Armada / Rances Colotto**

---

## 1. Visión general

El programa simula, **minuto a minuto**, la carga y despacho de vehículos en los tres
ferrys de la empresa *El Margariteño C.A.* durante un día completo (1 440 minutos).
Todo el código reside en un único archivo `main.c`, sin bibliotecas externas más allá
de las estándar requeridas por el enunciado.

La función `main` invoca `leer_datos` → `init_flota` → abre `proy1.out` en escritura →
`simular` → cierra el archivo de salida.

---

## 2. Estructuras de datos

### 2.1 `Veh` — Vehículo

Almacena exactamente los campos del archivo de entrada:

| Campo | Tipo | Descripción |
|-------|------|-------------|
| `cod` | `int` | Código de 3 dígitos (`c1 c2 c3`) |
| `npa` / `npt` | `int` | Pasajeros adultos / tercera edad (sin chofer) |
| `tpa` / `tpt` | `int` | Clase del pasaje (0 = VIP/1ra · 1 = Ejecutiva/Turista) |
| `peso` | `int` | Peso en kg (o toneladas si `c1 = 4`) |
| `hora` | `int` | Hora militar de llegada |
| `placa` | `char[16]` | Identificador del vehículo |
| `tipo_ferry` | `int` | Ferry destino: 0 = Express · 1 = Tradicional |

El código de 3 dígitos se descompone con aritmética entera:

```c
int tipo_v(const Veh *v)    { return v->cod / 100; }   /* c1: tipo */
int lleva_pas(const Veh *v) { return v->cod % 10;  }   /* c3: pasajeros */
```

### 2.2 `Cola` — Cola circular de índices

```
buf[0..MAX_VEH-1]   arco circular de índices en lista[]
head                 índice del primer elemento
tail                 índice del próximo espacio libre
size                 número de elementos actuales
```

Se emplean **cuatro** colas durante la simulación (nombres en el código):

| Cola | Contenido |
|------|-----------|
| `colaExpress` | Vehículos esperando el ferry Express (`tipo_ferry == 0`) |
| `colaTradicional` | Vehículos esperando un ferry Tradicional (`tipo_ferry == 1`) |
| `colaEmergencias` | Vehículos de emergencia (prioridad absoluta) |
| `colaMuelle` | Índices de ferrys en espera de turno en el muelle |

La operación `q_push_f` inserta al **frente** de la cola (para los vehículos que son
bajados del ferry por una emergencia y deben volver al principio de la fila).

### 2.3 `Ferry` — Estado de un barco

Cada ferry mantiene dos grupos de campos:

**Capacidades fijas** (invariantes):

| Campo | Descripción |
|-------|-------------|
| `cap_veh` / `min_veh` | Máximo y mínimo vehicular (30 % redondeado hacia arriba) |
| `cap_pas` | Pasajeros totales |
| `cap_vip` / `cap_eje` | Asientos VIP/1ra y Ejecutiva/Turista |
| `cap_ton` | Peso máximo en toneladas |
| `t_ciclo` | Duración de viaje + retorno (minutos) |

**Estado dinámico** (se reinicia cada vez que zarpa):

`n_veh`, `pas`, `pas_3ra`, `vip_us`, `eje_us`, `ton_car`, `estado`, `t_rest`

### 2.4 `Stats` — Estadísticas diarias

Acumula: vehículos transportados, pasajeros transportados, ingresos, pasajeros no
trasladados, vector `frec[]` por tipo de vehículo y el pico de espera (cantidad + hora).

**Nota sobre `frec[]`:** en `simular`, el arreglo se incrementa **al llegar** cada
vehículo al terminal y **otra vez** al contabilizar cada vehículo embarcado en un viaje
que zarpa. El informe *“Vehículo más frecuente”* usa ese acumulado tal como está
implementado en `rep_stats`.

---

## 3. Lectura de entrada (`leer_datos`)

1. Abre `proy1.in`.
2. Primera línea → `orden[3]`: el orden inicial de carga de los ferrys (IDs 1-3,
   convertidos a índices 0-2).
3. Líneas siguientes → cada campo leído con `sscanf`, 9 campos exactos.
4. Los vehículos se almacenan en orden de llegada (garantizado por el enunciado).

---

## 4. Inicialización de la flota (`init_flota`)

Carga los valores fijos de los tres barcos y reinicia todos los contadores dinámicos.
El mínimo de vehículos se pre-calcula con `ceil(cap_veh × 0.3)`:

| Ferry | Tipo | cap_veh | min_veh | cap_pas | cap_ton | t_ciclo |
|-------|------|--------:|--------:|--------:|--------:|--------:|
| Lilia Concepción | Express | 16 | 5 | 50 | 60 t | 35 min |
| La Isabela | Tradicional | 20 | 6 | 70 | 80 t | 65 min |
| La Margariteña | Tradicional | 18 | 6 | 60 | 80 t | 65 min |

---

## 5. Lógica de embarque

### 5.1 `puede_subir` — ¿Cabe el vehículo?

Comprueba **cinco** restricciones antes de embarcar:

1. No exceder el cupo vehicular (`n_veh < cap_veh`)
2. No exceder el total de pasajeros (`pas + nuevos ≤ cap_pas`)
3. No exceder los asientos VIP/1ra (`vip_us + nuevos_vip ≤ cap_vip`)
4. No exceder los asientos Ejecutiva/Turista (`eje_us + nuevos_eje ≤ cap_eje`)
5. No exceder el peso máximo (`ton_car + v_ton(v) ≤ cap_ton`)

El chofer **siempre** consume un asiento de la clase indicada por `tpa`.

### 5.2 `embarcar` / `desembarcar_cnt`

`embarcar` registra el índice del vehículo en `abordo[]` y actualiza todos los
contadores. `desembarcar_cnt` deshace esos cambios sin modificar `abordo[]`
(el array se trunca ajustando `n_veh` directamente al bajar un vehículo).

### 5.3 Prioridad de emergencias

Cuando llega un vehículo de emergencia (`c1 ∈ {5, 6, 7}`):

```
mientras ferry lleno en vehículos:
    bajar el último vehículo normal de abordo[]
    revertir sus contadores (desembarcar_cnt)
    reencolar al frente de su cola original (`q_push_f` hacia `colaExpress` o `colaTradicional`)
embarcar el vehículo de emergencia (sin comprobar `puede_subir`)
```

El vehículo de emergencia se embarca **sin** pasar por `puede_subir`, por lo que puede
dejar el ferry por encima de los límites de pasajeros o de peso si el enunciado lo
requiere en ese escenario.

---

## 6. Condición de zarpe (`puede_zarpar`)

El ferry zarpa si se cumple **cualquiera** de estas condiciones:

| Prioridad | Condición |
|-----------|-----------|
| 1 | `n_veh ≥ cap_veh` — lleno en vehículos |
| 2 | `pas ≥ cap_pas` — lleno en pasajeros |
| 3 | `n_veh ≥ min_veh` **y** cola vacía |
| 4 | `n_veh ≥ min_veh` **y** ningún vehículo en cola puede subir (`puede_subir` retorna 0 para todos) |

> Un ferry **no zarpa** si hay vehículos de emergencia esperando (`colaEmergencias` no vacía).

---

## 7. Bucle de simulación (`simular`)

Variables principales del reloj: `minutoActual`, `minutoFin = minutoActual + 1440`,
`minutoDisponibleCarga` (no se puede embarcar otro vehículo hasta pasados `T_CARGA`
minutos tras el último embarque), `ferryEnMuelle` (índice del ferry en el muelle o -1).

```
inicializar colas y estadísticas (statsDia)
llenar colaMuelle con el orden inicial de ferrys (archivo)
primer ferry al muelle; minutoDisponibleCarga = minutoActual

mientras minutoActual ≤ minutoFin:

  [1] Encolar vehículos cuya hora de llegada coincide con minutoActual
        emergencias → colaEmergencias
        tipo_ferry = 0 → colaExpress
        tipo_ferry = 1 → colaTradicional
        (y contar llegada en statsDia.frec[])

  [2] Si hay ferry en muelle y minutoActual ≥ minutoDisponibleCarga:
        colaFerry = colaExpress o colaTradicional según tipo del ferry
        prioridad 1: embarcar emergencia (bajando normales si el ferry está lleno en vehículos)
        prioridad 2: si la cola correspondiente no está vacía y puede_subir, embarcar frente
        si colaEmergencias vacía y puede_zarpar:
            acumular estadísticas del viaje e ingresos
            rep_viaje (stdout y proy1.out)
            reiniciar contadores del ferry; estado EN_VIAJE; t_rest = t_ciclo
            siguiente ferry desde colaMuelle (si hay)

  [2b] (rama aparte en el código) Si ferryEnMuelle < 0 y colaMuelle no está vacía:
        asignar el siguiente ferry al muelle (no depende de haber cargado en [2])

  [3] Estadística de espera máxima: tamaño colaExpress + colaTradicional + colaEmergencias
      (se calcula DESPUÉS de la carga de este minuto, como en el código)

  [4] Para cada ferry en EN_VIAJE: decrementar t_rest; si llega a 0 → EN_ESPERA y encolar en colaMuelle

  [5] Condición de fin:
        si ya no quedan vehículos por leer
        y las tres colas de espera están vacías
        y NO hay un ferry en muelle con n_veh ≥ min_veh (capaz de seguir la simulación)
          → salir del bucle

  minutoActual++
```

---

## 8. Cálculo de pasajeros no trasladados

Al terminar la simulación se cuentan como `pas_perd`:

1. **Vehículos en `colaEmergencias`, `colaExpress`, `colaTradicional`** que nunca abordaron.
2. **Vehículos en el ferry del muelle** que no alcanzaron el mínimo para zarpar
   (el ferry quedó "a medio cargar" al acabar el día).

Para cada vehículo: `pas_perd += 1 (chofer) + npa + npt` si `c3 = 1`, o solo `+1`
si `c3 = 0`.

---

## 9. Cálculo de ingresos (`ing_veh` + `ing_pas`)

### Tarifa vehicular

| c1 | Express | Tradicional |
|----|--------:|------------:|
| 1, 5, 6, 7 (liviano/emergencia) | 1 090 BsF | 590 BsF |
| 2 (rústico) | 1 310 BsF | 710 BsF |
| 3 (microbús) | 1 600 BsF | 850 BsF |
| 4 (carga) | 2 400 BsF | 1 200 BsF |

### Tarifa de pasajeros

Siempre se cobra el **chofer** (clase según `tpa`). Si `c3 = 1`:
- `npa` adultos a tarifa `tpa`
- `npt` de tercera edad a tarifa `tpt`

| Clase | Express Adulto | Express Tercera edad | Trad. Adulto | Trad. Tercera edad |
|-------|---------------:|---------------------:|-------------:|-------------------:|
| VIP / 1ra | 1 020 BsF | 520 BsF | 370 BsF | 190,50 BsF |
| Ejecutiva / Turista | 620 BsF | 320 BsF | 290 BsF | 150,50 BsF |

---

## 10. Salida

Las funciones `rep_viaje` y `rep_stats` escriben en un arreglo `destinos[]` con
`stdout` y el `FILE *` de salida. Cada zarpe imprime (pantalla **y** `proy1.out`):

```
Carga Nro. N:
Ferry <nombre>
Viaje Nro. M
Num. vehiculos: X
Num. pasajeros: Y (Z mayores de 60 anos)
Peso: W.WW toneladas
Ingreso: IIII BsF.
ID: <placa>     Tipo: <tipo>
...
```

Al final del día se imprimen las estadísticas globales:

```
Estadisticas:
Total vehiculos transportados: ...
Total pasajeros transportados: ...
Total de ingresos: ... BsF.
Num. pasajeros no trasladados: ...
Vehiculo mas frecuente: ...
Mayor frec. vehiculos en espera: N (a las HHMM)
```
---

> Los archivos `proy1` (ejecutable) y `proy1.out` están en `.gitignore`.
