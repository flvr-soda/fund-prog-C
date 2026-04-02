# Solución — Simulador de Ferrys Margarita
**TAO207 · Sección 08 · Ismael Armada / Rances Colotto**

---

## 1. Visión general

El programa simula, **minuto a minuto**, la carga y despacho de vehículos en los tres
ferrys de la empresa *El Margariteño C.A.* durante un día completo (1 440 minutos).
Todo el código reside en un único archivo `proy1.c`, sin bibliotecas externas más allá
de las estándar requeridas por el enunciado.

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

Se emplean **cuatro** colas durante la simulación:

| Cola | Contenido |
|------|-----------|
| `q_exp` | Vehículos esperando el ferry Express |
| `q_trad` | Vehículos esperando un ferry Tradicional |
| `q_emerg` | Vehículos de emergencia (prioridad absoluta) |
| `q_muelle` | Índices de ferrys disponibles para cargar |

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

Acumula: vehículos totales, pasajeros totales, ingresos, pasajeros perdidos,
frecuencia por tipo de vehículo y el pico de espera (cantidad + hora).

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
    reencolar al frente de su cola original (q_push_f)
embarcar el vehículo de emergencia
```

El vehículo de emergencia puede **sobrepasar** los límites de pasajeros y peso
(overflow intencionado según el enunciado).

---

## 6. Condición de zarpe (`puede_zarpar`)

El ferry zarpa si se cumple **cualquiera** de estas condiciones:

| Prioridad | Condición |
|-----------|-----------|
| 1 | `n_veh ≥ cap_veh` — lleno en vehículos |
| 2 | `pas ≥ cap_pas` — lleno en pasajeros |
| 3 | `n_veh ≥ min_veh` **y** cola vacía |
| 4 | `n_veh ≥ min_veh` **y** ningún vehículo en cola puede subir (`puede_subir` retorna 0 para todos) |

> Un ferry **no zarpa** si hay vehículos de emergencia esperando (`q_emerg` no vacía).

---

## 7. Bucle de simulación (`simular`)

```
inicializar colas y reloj
primer ferry al muelle

mientras min_act ≤ min_fin:

  [1] encolar vehículos que llegan en min_act
        emergencias → q_emerg
        tipo_ferry=0 → q_exp
        tipo_ferry=1 → q_trad

  [2] actualizar estadística de espera máxima

  [3] si hay ferry en muelle y t_libre ≤ min_act:
        prioridad 1: embarcar emergencia (bajando normales si es necesario)
        prioridad 2: embarcar frente de la cola del ferry
        
        si puede_zarpar:
            acumular estadísticas del viaje
            imprimir reporte del viaje
            reiniciar contadores del ferry
            poner ferry en EN_VIAJE con t_rest = t_ciclo
            sacar siguiente ferry de q_muelle (si existe)

  [4] decrementar t_rest de los ferrys en viaje
        si t_rest ≤ 0 → pasar a EN_ESPERA, encolar en q_muelle

  [5] condición de fin:
        si no hay más vehículos por llegar
        y las tres colas de espera están vacías
        y el ferry en muelle no alcanza el mínimo
          → terminar

  min_act++
```

---

## 8. Cálculo de pasajeros no trasladados

Al terminar la simulación se cuentan como `pas_perd`:

1. **Vehículos en `q_emerg`, `q_exp`, `q_trad`** que nunca abordaron.
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

Cada zarpe imprime (pantalla **y** `proy1.out`):

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
