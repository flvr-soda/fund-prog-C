## UNIVERSIDAD DE CARABOBO
Facultad Experimental de Ciencias y Tecnología
Departamento de Computación — Fundamentos de Programación

Cod. Asignatura: TAO207

## Proyecto (valor: 20%) — Ferrys Margarita

---

## Normas de Entrega

La entrega del proyecto se realizará durante la última semana del semestre, en la fecha y lugar
que serán anunciados previamente por el profesor.

---

## 1. Contenido

Se deberá entregar un archivo comprimido en formato `.zip` que contenga:

- Un único archivo fuente con extensión `.c`, que incluya todo el desarrollo del proyecto.
- Un informe cuyo contenido se describe en las páginas subsiguientes.
- Los archivos de entrada correspondientes.
- No deben incluirse archivos de salida ni ejecutables generados por el compilador (gcc).

---

## 2. Codificación

- El nombre del archivo `.c` y el de los archivos de entrada debe coincidir exactamente
  con lo indicado en el enunciado del problema.
- El archivo fuente debe contener en su cabecera, mediante comentario de bloque, la
  siguiente información:
  - Fecha
  - Nombre y Apellido
  - Cédula
  - Número de Sección

---

## 3. Defensa oral (Obligatoria)

- Cada equipo deberá realizar una defensa oral del proyecto, en la fecha y horario
  establecidos por el profesor.
- Durante la defensa, los integrantes deberán explicar el funcionamiento del programa,
  las decisiones de diseño tomadas, y responder preguntas sobre su implementación.

---

## 4. Informe

Se debe entregar un informe en formato digital, incluido en el archivo comprimido, que
contenga los siguientes apartados:

### 4.1. Portada
- Título del proyecto
- Nombres y apellidos de los integrantes
- Cédulas de identidad
- Sección
- Fecha de entrega

### 4.2. Descripción del problema
- Explicación general del enunciado y objetivos
- Contexto y propósito del software desarrollado

### 4.3. Análisis de la solución
- Cómo se organizan y almacenan los datos
- Justificación de las decisiones tomadas

### 4.4. Estructuras de datos
- Definición de los structs utilizados
- Arreglos u otras estructuras aplicadas
- Diagrama o representación opcional

### 4.5. Algoritmos implementados
- Explicación en lenguaje pseudoformal
- Puede usarse pseudocódigo, texto estructurado o diagramas

### 4.6. Pruebas realizadas
- Descripción de los casos de prueba
- Breve resumen del comportamiento observado

### 4.7. Conclusiones
- Reflexión sobre la experiencia de desarrollo
- Dificultades encontradas y soluciones
- Posibles mejoras

El informe debe estar redactado en letra Arial o Calibri (Word) o Roman (LaTeX), tamaño 12
puntos, con márgenes de 3 cm a la izquierda y 2 cm en los demás bordes.

---

## 5. Importante

- El proyecto debe ser realizado en equipos de 2 (dos) estudiantes de la misma sección
  de laboratorio, sin excepción. No se aceptarán cambios de equipos una vez conformados.
- El programa debe desarrollarse exclusivamente en lenguaje ANSI C, utilizando las
  librerías estándar: `stdio.h`, `ctype.h`, `string.h`, `stdlib.h`, `assert.h`, `time.h` y `math.h`.
- La solución debe aplicar únicamente los contenidos vistos en clase, tales como:
  Condicionales, Ciclos, Acciones nominadas, Archivos, Arreglos y Registros.
- Es obligatorio el uso de acciones nominadas (subrutinas con nombres significativos) y
  parámetros en el código.
- El código debe estar bien documentado y estructurado, con comentarios, indentación
  clara y uso de nombres significativos. Considere seguir algún formato de documentación
  compatible con herramientas ampliamente utilizadas tales como
  [doxygen](https://www.doxygen.nl/), o en su lugar,
  [sphinx](https://www.sphinx-doc.org/en/master/).
- El informe será evaluado también en cuanto a su redacción y ortografía.
- Los proyectos entregados después de la fecha tope tendrán una penalización de 5
  puntos por cada día de retraso.
- Los criterios de evaluación serán publicados oportunamente por el profesor.

No seguir las indicaciones antes mencionadas repercutirá en la nota final de su entrega.

---

## 1. Enunciado

La empresa **El Margariteño C.A.** tiene una flota naviera que contribuye a impulsar el desarrollo
turístico de la Isla de Margarita. Cuenta con una flota de 2 tipos de ferry: Ferrys Express y Ferrys
Tradicionales. Desde el año 2005 han trasladado miles de personas entre Puerto La Cruz y la Isla
de Margarita, proporcionando confort a los viajeros durante su viaje.

A diario la empresa realiza viajes a la isla de manera continua, transportando vehículos y
pasajeros. El objetivo de este proyecto es simular el proceso de carga y transporte de vehículos
en los ferrys durante un día, recolectando ciertas estadísticas en el proceso. Se asume que la
empresa sólo hace viajes desde Puerto La Cruz hacia Margarita, pero no de regreso.

Actualmente la empresa cuenta con tres ferrys. El ferry **"Lilia Concepción"**, de tipo Express, tiene
una capacidad de 16 vehículos y 50 pasajeros (30 en clase ejecutiva y 20 VIP). Los Ferrys
Tradicionales corresponden a 2 barcos: **"La Isabela"**, con capacidad de 20 vehículos y 70 pasajeros
(50 en clase turista y 20 en primera clase) y el ferry **"La Margariteña"** que puede transportar 18
vehículos y 60 pasajeros (40 en clase turista y 20 en primera clase).

La empresa naviera ha establecido unas tarifas para trasladar a las personas desde Puerto La Cruz
hasta la Isla de Margarita, tal como se indican a continuación:

| Tipo de pasaje                          | Convencional (Bs.F.) | Express (Bs.F.) |
|-----------------------------------------|---------------------:|----------------:|
| **Primera clase / VIP — Adultos***      | 370,00               | 1020,00         |
| Primera clase / VIP — Tercera edad (>60)| 190,50               | 520,00          |
| **Turista / Ejecutiva — Adultos***      | 290,00               | 620,00          |
| Turista / Ejecutiva — Tercera edad (>60)| 150,50               | 320,00          |
| **Vehículo liviano**                    | 590,00               | 1090,00         |
| **Vehículo rústico**                    | 710,00               | 1310,00         |
| **Microbus / Van**                      | 850,00               | 1600,00         |
| **Vehículo de Carga**                   | 1200,00              | 2400,00         |

> \* Ferry Tradicional → "Primera clase" / "clase turista"  
> \*\* Ferry Express → "VIP" / "clase ejecutiva"  
> \*\*\* Niños menores de 12 años viajan gratis (no se cobran ni se cuentan como pasajeros)

> **Nota sobre pasajeros:** El chofer del vehículo **no** es considerado un pasajero en los campos
> de entrada (`NPa`, `NPt`), pero **sí** debe contabilizarse en las estadísticas como pasajero adulto
> (menor de 60 años). Todo vehículo lleva exactamente un chofer.

Debido a que la empresa está en proceso de expansión, en estos momentos sólo cuenta con un
muelle para realizar las cargas. Por lo tanto, los vehículos deben organizarse en la terminal
realizando **2 colas**: una para el ferry tradicional y otra para el ferry Express.

El proceso de carga de un ferry ocurre al mismo tiempo que la llegada de vehículos a las colas
organizadas en la terminal. Esto quiere decir que, en cada instante de tiempo, puede estar
ingresando un vehículo al ferry y, simultáneamente, puede llegar un vehículo a la terminal. Se
asume que toma **3 minutos exactos** cargar o descargar un vehículo al ferry y que los tiempos de
llegada de los vehículos a las colas de espera no contienen fracciones de minutos.

La empresa le proporciona un trato prioritario a los **vehículos de emergencia**
(ambulancias, bomberos y policías), ya que se les debe garantizar el ingreso al ferry que esté en
proceso de carga al momento de su llegada; este tipo de vehículos tendrá prioridad absoluta para
ingresar al ferry. Sin embargo, esta situación puede generar un desbordamiento de la capacidad
del ferry, por lo cual en ocasiones será necesario bajar del ferry uno o más vehículos que hayan
ingresado al mismo y colocarlos nuevamente de primeros en la respectiva cola de espera.

Cada uno de los ferrys que conforman la flota naviera dispone de una **banda única** en la cual se
transportan los vehículos formando una cola. Al momento de cargar el ferry se debe tomar en
cuenta el peso máximo de carga que puede soportar cada ferry; el tipo Express soporta **60
toneladas** y los de tipo tradicional pueden transportar hasta **80 toneladas** cada uno.

> **Nota sobre el peso:** Los vehículos de carga (`c1 = 4`) tienen su peso expresado directamente
> en **toneladas**. El resto de los vehículos tienen su peso en **kilogramos** y deben convertirse
> dividiendo entre 1000 al comparar con el peso máximo del ferry.

Por razones de costo, un ferry **no puede iniciar un viaje** si no ha embarcado por lo menos el **30%
de su capacidad vehicular**. De forma que el ferry puede partir si:
- Tiene ocupada su capacidad mínima de carga (número mínimo de vehículos) y no hay
  vehículos esperando para ingresar al mismo (no hay vehículos en cola), o
- Si ya ha alcanzado la capacidad máxima de vehículos y/o pasajeros.
- También podrá zarpar si no pueden ingresar más vehículos de los que están en cola
  sin sobrepasar el peso que puede transportar (peso máximo).

> **Nota sobre el mínimo de vehículos:** El 30% se calcula con redondeo hacia arriba
> (techo). Por ejemplo: 30% de 16 = 4,8 → mínimo = 5.

El proceso de simulación culmina cuando las colas de acceso al ferry estén vacías y no existan
suficientes vehículos dentro del ferry para iniciar un viaje.

Por otro lado, se conoce que los ferrys tienen diversas velocidades. Los ferrys tradicionales viajan
a una velocidad de 17 nudos y el ferry Express a una velocidad promedio de 40–50 nudos. Para
efectos de esta simulación, considere que los ferrys tradicionales tardan **65 minutos** en trasladar
los vehículos y retornar al terminal de ferrys, mientras que el ferry Express demora **35 minutos**.

En todo momento los ferrys solamente podrán estar en alguno de los siguientes estados:

| Estado | Descripción        |
|-------:|--------------------|
| 1      | En carga           |
| 2      | En viaje           |
| 3      | En espera para cargar |

---

## 2. Actividades a realizar

Desarrolle un programa que simule el proceso de traslado durante un día. El programa debe
simular el paso del tiempo (minuto a minuto) y representar:
- La cola de vehículos dentro del ferry
- Las colas de vehículos en espera para abordar el ferry Express y el Tradicional
- La cola de ferrys que están en el muelle en espera para cargar vehículos
- Los vehículos que aún no han llegado al muelle

El programa debe reportar **para cada viaje** lo siguiente:
- Nombre del ferry.
- Número de vehículos transportados.
- Cuántos pasajeros fueron transportados. Cuántos de ellos son mayores de 60 años.
- Peso total transportado (en toneladas).
- Ingreso en BsF.
- Datos de los vehículos transportados (placa y tipo).

Además se debe calcular al final del día:
- Total de vehículos y pasajeros transportados en el día.
- Total de ingresos del día en BsF.
- Número de pasajeros que no pudieron ser trasladados al finalizar el día.
- El tipo de vehículo que viaja con mayor frecuencia a la isla.
- Cuál fue la mayor frecuencia de vehículos en espera y a qué hora del día.

---

## 3. Formato de entrada y salida

Los datos de entrada se deben cargar desde un archivo de texto llamado `proy1.in`, el cual consiste
en una serie de líneas, cada una de las cuales representa un vehículo, con la siguiente
información:

```
〈Cod, NPa, NPt, tpa, tpt, P, Hr, ID, f〉
```

### Campos:

| Campo | Descripción |
|-------|-------------|
| `Cod` | Código del vehículo (3 dígitos: `c1 c2 c3`). |
| `NPa` | Entero en el rango [0..20]. Número de pasajeros adultos (sin contar chofer) que traslada el vehículo. |
| `NPt` | Entero en el rango [0..20]. Número de pasajeros de tercera edad (>60 años) que traslada el vehículo. |
| `tpa` | Tipo de pasaje adulto: `0` = primera clase/VIP, `1` = turista/ejecutiva. |
| `tpt` | Tipo de pasaje tercera edad: `0` = primera clase/VIP, `1` = turista/ejecutiva. |
| `P`   | Peso del vehículo. En vehículos de carga (`c1=4`): toneladas. En el resto: kilogramos. |
| `Hr`  | Hora de llegada en hora militar (ej: `200`=2:00 a.m., `930`=9:30 a.m., `2300`=23:00=11:00 p.m.). |
| `ID`  | Placa del vehículo (ej: `CH540BA`). |
| `f`   | Tipo de ferry destino: `0` = Express, `1` = Tradicional. |

### Detalle del campo `Cod`:

- **`c1`** — Tipo de vehículo:
  - `1`: familiar liviano
  - `2`: vehículo rústico
  - `3`: microbus/VAN
  - `4`: vehículo de carga
  - `5`: ambulancia
  - `6`: camión de bomberos
  - `7`: vehículo policial
- **`c2`** — Zona de procedencia:
  - `0`: Estado Nueva Esparta
  - `1`: Estado Anzoátegui
  - `2`: otros estados
- **`c3`** — ¿Traslada pasajeros?:
  - `1`: sí
  - `0`: no

> Si `c3 = 0` (no traslada pasajeros), los campos `NPa`, `NPt`, `tpa` y `tpt` son ignorados.
> El chofer del vehículo **no** es considerado pasajero en la entrada, pero siempre debe
> contabilizarse como pasajero adulto en las estadísticas.

El archivo de entrada inicia con una **primera línea** que indica el orden en que inicia la carga
de los ferrys. El Ferry Lilia Concepción se identifica con el número `1`, La Isabela con el número `2`
y La Margariteña corresponde al número `3`.

> **Nota:** Los vehículos en el archivo están organizados por hora de llegada.
> La simulación inicia a la hora de llegada del primer vehículo.

Los resultados deben visualizarse por pantalla y almacenarse en un archivo de texto llamado
`proy1.out`. El formato de la salida debe coincidir con el ejemplo de salida de la siguiente sección.

---

## 4. Ejemplos de entrada y salida

### Ejemplo de Entrada (`proy1.in`)

```
1 3 2
121  2  1  0  1  1800  800  CD345CC  0
300  0  0  0  0  4800  801  AE786CA  1
711  1  0  1  0  3000  801  EV677VU  0
400  0  0  0  0  12    804  XE789FA  0
511  0  1  0  0  3000  805  VY778TS  1
111  3  0  1  0  1900  805  TR900AA  1
210  0  0  0  0  2900  806  GT344FC  0
401  1  0  1  0  15    806  XE787GE  0
```

### Ejemplo de Salida (`proy1.out`)

```
Carga Nro. 1:
Ferry Lilia Concepcion
Viaje Nro. 1
Num. vehiculos: 6
Num. pasajeros: 12 (2 mayores de 60 anos)
Peso: 37.70 toneladas
Ingreso: 1018 BsF.
ID: CD345CC     Tipo: liviano
ID: EV677VU     Tipo: policial
ID: XE789FA     Tipo: carga
ID: VY778TS     Tipo: ambulancia
ID: GT344FC     Tipo: rustico
ID: XE787GE     Tipo: carga

Estadisticas:
Total vehiculos transportados: 6
Total pasajeros transportados: 12
Total de ingresos: 1018 BsF.
Num. pasajeros no trasladados: 5
Vehiculo mas frecuente: liviano
Mayor frec. vehiculos en espera: 5 (a las 0806)
```

### Descripción de las primeras líneas del archivo de entrada

**Primera línea:** `1 3 2`

Esta primera línea indica el orden en el cual serán cargados los ferrys al inicio de la simulación.
En el ejemplo se inicia con el primer ferry (Lilia Concepción), luego el tercero (La Margariteña) y
por último el segundo ferry (La Isabela).

**Segunda línea:** `121  2  1  0  1  1800  800  CD345CC  0`

Se refiere a un vehículo con la siguiente descripción:

| Campo | Valor    | Descripción                                                    |
|-------|----------|----------------------------------------------------------------|
| Cod   | `121`    | Vehículo liviano, procedente de otro estado, con pasajeros     |
| NPa   | `2`      | Traslada 2 pasajeros adultos (adicional al chofer)             |
| NPt   | `1`      | Traslada 1 adulto de la tercera edad (>60 años)                |
| tpa   | `0`      | Los adultos viajan en VIP                                      |
| tpt   | `1`      | La persona de tercera edad viaja en clase ejecutiva            |
| P     | `1800`   | El peso del vehículo es de 1800 kg                             |
| Hr    | `800`    | Hora de llegada: 8:00 a.m.                                     |
| ID    | `CD345CC`| Placa del vehículo                                             |
| f     | `0`      | Viaja en el ferry tipo Express                                 |

---

*"Como toda estrategia bien ejecutada, cada segundo cuenta.*
*La sabiduría comienza en la reflexión. Nada es verdad, todo está permitido."*
— E. A., AC II