

## UNIVERSIDAD DE CARABOBO
Facultad Experimental de Ciencias y Tecnología
Departamento de Computación Fundamentos de
## Programación

Cod. Asignatura: TAO207
## Proyecto (valor: 20%)
## Ferrys Margarita


Normas de Entrega:

La entrega del proyecto se realizará durante la última semana del semestre, en la fecha y lugar
que serán anunciados previamente por el profesor.

## 1. Contenido

Se deberá entregar un archivo comprimido en formato .zip que contenga:

● Un único archivo fuente con extensión .c, que incluya todo el desarrollo del proyecto.
● Un informe cuyo contenido se describe en las páginas subsiguientes.
● Los archivos de entrada correspondientes.
● No deben incluirse archivos de salida ni ejecutables generados por el compilador
## (gcc).

## 2. Codificación

● El nombre del archivo .c y el de los archivos de entrada debe coincidir exactamente
con lo indicado en el enunciado del problema.
● El archivo fuente debe contener en su cabecera, mediante comentario de bloque, la
siguiente información: o Fecha o Nombre y Apellido o Cédula
o Número de Sección

- Defensa oral (Obligatoria)

● Cada equipo deberá realizar una defensa oral del proyecto, en la fecha y horario
establecidos por el profesor.
● Durante la defensa, los integrantes deberán explicar el funcionamiento del programa,
las decisiones de diseño tomadas, y responder preguntas sobre su implementación.

## 4. Informe

Se debe entregar un informe en formato digital, incluido en el archivo comprimido, que
contenga los siguientes apartados:

## 4.1. Portada
● Título del proyecto
● Nombres y apellidos de los integrantes
● Cédulas de identidad
## ● Sección
● Fecha de entrega

4.2. Descripción del problema
● Explicación general del enunciado y objetivos
● Contexto y propósito del software desarrollado

4.3. Análisis de la solución
● Cómo se organizan y almacenan los datos
● Justificación de las decisiones tomadas

4.4. Estructuras de datos
● Definición de los structs utilizados
● Arreglos u otras estructuras aplicadas
● Diagrama o representación opcional

4.5. Algoritmos implementados
● Explicación en lenguaje pseudoformal
● Puede usarse pseudocódigo, texto estructurado o diagramas

4.6 Pruebas realizadas
● Descripción de los casos de prueba
● Breve resumen del comportamiento observado

## 4.7 Conclusiones
● Reflexión sobre la experiencia de desarrollo
● Dificultades encontradas y soluciones
● Posibles mejoras

El informe debe estar redactado en letra Arial o Calibri (Word) o Roman (LaTeX), tamaño 12
puntos, con márgenes de 3 cm a la izquierda y 2 cm en los demás bordes.

## 5. Importante

● El proyecto debe ser realizado en equipos de 2 (dos) estudiantes de la misma sección
de  laboratorio, sin  excepción.  No  se  aceptarán  cambios  de  equipos  una  vez
conformados.

● El programa debe desarrollarse exclusivamente en lenguaje ANSI C, utilizando las
librerías estándar: stdio.h, ctype.h, string.h, stdlib.h, assert.h, time.h y math.h.
● La solución debe aplicar únicamente los contenidos vistos en clase, tales como:
Condicionales, Ciclos, Acciones nominadas, Archivos, Arreglos y Registros.
● Es obligatorio el uso de acciones nominadas (subrutinas con nombres significativos) y
parámetros en el código.
● El código debe estar bien documentado y estructurado, con comentarios, indentación
clara y uso de nombres significativos. Considere seguir algún formato de documentación
compatible  con  herramientas  ampliamente  utilizadas  tales  como doxygen
(https://www.doxygen.nl/), o en su lugar, considere utilizar sphynx (https://www.sphinx-
doc.org/en/master/).
● El informe será evaluado también en cuanto a su redacción y ortografía.
● Los proyectos entregados después de la fecha tope tendrán una penalización de 5
puntos por cada día de retraso.
● Los criterios de evaluación serán publicados oportunamente por el profesor.

No seguir las indicaciones antes mencionadas repercutirá en la nota final de su entrega.






























## 1. Enunciado

La empresa El Margariteño C.A. tiene una flota naviera que contribuye a impulsar el desarrollo
turístico de la Isla de Margarita. Cuenta con una flota de 2 tipos de ferry: Ferrys Express y Ferrys
Tradicionales. Desde el año 2005 han trasladado miles de personas entre Puerto La Cruz y la Isla
de Margarita, proporcionando confort a los viajeros durante su viaje.

A diario la empresa realiza viajes a la isla de manera continua, transportando vehículos y
pasajeros. El objetivo de este proyecto es simular el proceso de carga y transporte de vehículos
en los ferrys durante un día, recolectando ciertas estadísticas en el proceso. Se asume que la
empresa sólo hace viajes desde Puerto La Cruz hacia Margarita, pero no de regreso.

Actualmente la empresa cuenta con tres ferrys. El ferry “Lilia Concepción”, de tipo Express, tiene
una capacidad de 16 vehículos y 50 pasajeros (30 en clase ejecutiva y 20 VIP). Los Ferrys
Tradicionales corresponden a 2 barcos: “La Isabela”, con capacidad de 20 vehículos y 70 pasajeros
(50 en clase turista y 20 en primera clase) y el ferry “La Margariteña” que puede transportar 18
vehículos y 60 pasajeros (40 en clase turista y 20 en primera clase).

La empresa naviera ha establecido unas tarifas para trasladar a las personas desde Puerto La Cruz
hasta la Isla de Margarita, tal como se indican a continuación:

Tipo de pasaje  Precios (Bs.F.)
Primera clase*/VIP**  Convencional  Express
## Adultos ***  370,00   1020,00
Tercera edad (mayores de 60 años)  190,50   520,00
Turista*/Ejecutiva**

## Adultos ***  290,00   620,00
Tercera edad (mayores de 60 años)  150,50   320,00
## Vehículos

Vehículo liviano  590,00   1090,00
Vehículo rústico  710,00   1310,00
## Microbus / Van  850,00  1600,00
Vehículo de Carga  1200,00   2400,00

-  Se refiere a Ferry Tradicional   **  Se refiere a Ferry Express    ***
Niños menores de 12 años viajan  gratis

Debido a que la empresa está en proceso de expansión, en estos momentos sólo cuenta con un
muelle para realizar las cargas. Por lo tanto, los vehículos deben organizarse en la terminal
realizando 2 colas, una para el ferry tradicional y otra para el ferry Express.


El proceso de carga de un ferry ocurre al mismo tiempo que la llegada de vehículos a las colas
organizadas en la terminal. Esto quiere decir que, en cada instante de tiempo, puede estar
ingresando un vehículo al ferry y, simultáneamente, puede llegar un vehículo a la terminal. Se
asume que toma 3 minutos exactos cargar o descargar un vehículo al ferry y que los tiempos de
llegada de los vehículos a las colas de espera no contienen fracciones de minutos.

Ahora bien, la empresa le proporciona un trato prioritario a los vehículos de emergencia
(ambulancias, bomberos y policías), ya que se les debe garantizar el ingreso al ferry que esté en
proceso de carga al momento de su llegada; este tipo de vehículos tendrá prioridad absoluta para
ingresar al ferry. Sin embargo, esta situación puede generar un desbordamiento de la capacidad
del ferry, por lo cual en ocasiones será necesario bajar del ferry uno o más vehículos que hayan
ingresado al mismo y colocarlos nuevamente de primeros en la respectiva cola de espera.

Cada uno de los ferrys que conforman la flota naviera dispone de una banda única en la cual se
transportan los vehículos formando una cola. Al momento de cargar el ferry se debe tomar en
cuenta el peso máximo de carga que puede soportar cada ferry; el tipo Express soporta 60
toneladas y los de tipo tradicional pueden transportar hasta 80 toneladas cada uno.

Por razones de costo, un ferry no puede iniciar un viaje si no ha embarcado por lo menos el 30%
de su capacidad vehicular. De forma que el ferry puede partir si tiene ocupada su capacidad
mínima de carga (número mínimo de vehículos) y no hay vehículos esperando para ingresar al
mismo (no hay vehículos en cola), o si ya ha alcanzado la capacidad máxima de vehículos y/o
pasajeros. También podrá zarpar si no pueden ingresar más vehículos de los que están en cola
sin sobrepasar el peso que puede transportar (peso máximo). El proceso de simulación culmina
cuando las colas de acceso al ferry estén vacías y no existan suficientes vehículos dentro del ferry
para iniciar un viaje.

Por otro lado, se conoce que los ferrys tienen diversas velocidades. Los ferrys tradicionales viajan
a una velocidad de 17 nudos y el ferry Express a una velocidad promedio de 40-50 nudos. Para
efectos de esta simulación, considere que los ferrys tradicionales tardan 65 minutos en trasladar
los vehículos y retornar al terminal de ferrys, mientras que el ferry Express demora 35 minutos.

En todo momento los ferrys solamente podrán estar en alguno de los siguientes estados: en carga
(estado = 1), en viaje (estado = 2) o en espera para cargar (estado = 3).



- Actividades a realizar

Desarrolle un programa que simule el proceso de traslado durante un día. El programa debe
simular el paso del tiempo (minuto a minuto) y representar la cola de vehículos dentro del ferry,
las colas de vehículos en espera para abordar el ferry Express y el Tradicional, la cola de ferrys
que están en el muelle en espera para cargar vehículos y los vehículos que aún no han llegado al
muelle.


El programa debe reportar para cada viaje lo siguiente:
- Nombre del ferry.
- Número de vehículos transportados.
- Cuántos pasajeros fueron transportados. Cuántos de ellos son mayores de 60 años.
- Peso total transportado (en toneladas).
- Ingreso en BsF.
- Datos de los vehículos transportados (placa y tipo).

Además se debe calcular:
- Total de vehículos y pasajeros transportados en el día.
- Total de ingresos del día en BsF.
- Número de pasajeros que no pudieron ser trasladados al finalizar el día.
- El tipo de vehículo que viaja con mayor frecuencia a la isla.
- Cuál fue la mayor frecuencia de vehículos en espera y a qué hora del día.

- Formato de entrada y salida

Los datos de entrada se deben cargar desde un archivo de texto llamado proy1.in, el cual consiste
en una serie de líneas, cada una de las cuales representa un vehículo, con la siguiente
información:
〈Cod, NPa, NPn, tpa, tpt, P, Hr, ID, f〉

## Donde:

Cod:  Código del vehículo. Está representado por un número de 3 dígitos (c
## 1
c
## 2
c
## 3
). c
## 1

representa el tipo de vehículo, un valor 1 indica que es de tipo familiar liviano; 2 un
vehículo rústico; 3 un microbus/VAN; 4 un vehículo de carga; 5 indica que es una
ambulancia, 6 un camión de bomberos y 7 un vehículo policial. El segundo dígito (c
## 2
## )
representa la zona de procedencia del vehículo, c
## 2
= 0 representa el Estado Nueva
Esparta, 1 el Estado Anzoátegui y 2 otros estados. El tercer dígito (c
## 3
) informa si traslada
pasajeros (c
## 3
= 1) o no (c
## 3
= 0). El chofer del vehículo no es considerado un pasajero, pero
todo vehículo lleva un chofer y por lo tanto debe ser contabilizado al generar las
estadísticas. Para efectos del proyecto el chofer es un adulto menor de 60 años.
NPa
Entero en el rango [1..20]. Señala el número de pasajeros adultos que traslada el
vehículo.
NPt
Entero en el rango [1..20]. Señala el número de pasajeros de tercera edad que traslada el
vehículo.
tpa
Se refiere al tipo de pasaje adquirido por los adultos. Un valor de 0 indica primera clase
o VIP y un valor de 1 se refiere a clase turista o ejecutiva.

tpt
Se refiere al tipo de pasaje adquirido por los pasajeros de tercera edad. Un valor de 0
indica primera clase  o VIP y un valor de 1 se refiere a clase turista o ejecutiva.
## P:
Peso del vehículo. En los vehículos de carga el peso viene expresado en toneladas, en el
resto de los vehículos se indica en kilogramos.
## Hr:
Hora de llegada (en hora militar). Ejemplo: 200 = 2:00 a.m., 930 = 9:30 a.m., 2300 = 11:00
p.m., 2350 = 11:50 p.m.
## ID:
Placa del vehículo. Ejemplo: CH540BA
f:
Tipo de ferry (0 = Express, 1 = Tradicional)

El archivo de entrada inicia con una primera línea que indica el orden en que inicia la carga
de los ferrys. Considere que el Ferry Lilia Concepción se identifica con el número 1, el ferry
La Isabela con el número 2 y La Margariteña corresponde al número 3.

Nota: Asuma que los vehículos reflejados en el archivo se encuentran organizados por la hora
de llegada. La simulación inicia a la hora de llegada del primer vehículo.


- Datos de salida

Los resultados deben visualizarse por pantalla y almacenarse en un archivo de texto llamado
proy1.out. El formato de la salida debe coincidir con el ejemplo de salida de la siguiente sección.

- Ejemplos de entrada y salida


Ejemplo de Entrada

Ejemplo de Salida
## 1 3 2

## Carga Nro. 1:
121  2  1  0  1  1800 800  CD345CC  0  Ferry Lilia Concepcion
300  0  0  0  0  4800 801  AE786CA  1  Viaje Nro. 1
711  1  0  1  0  3000 801  EV677VU  0  Num. vehiculos: 6
400  0  0  0  0  12  804  XE789FA  0  Num. pasajeros: 12 (2 mayores de edad)
511  0  1  0  0  3000 805  VY778TS  1  Peso: 37.70 toneladas
111  3  0  1  0  1900 805  TR900AA  1  Ingreso: 1018 BsF.
210  0  0  0  0  2900 806  GT344FC  0  ID: CD345CC     Tipo: liviano

## 401












## 1












## 0












## 1












## 0












## 15  806












## XE787GE












## 0












ID: EV677VU     Tipo: policial
ID: XE789FA     Tipo: carga
ID: VY778TS     Tipo: ambulancia
ID: GT344FC     Tipo: rustico
ID: XE787GE     Tipo: carga

## Estadisticas:
Total vehiculos transportados: 6 Total
pasajeros transportados: 12 Total de
ingresos: 1018 BsF.
Num. pasajeros no trasladados: 5
Vehiculo mas frecuente: liviano
Mayor frec. vehiculos en espera: 5 (a las 0806)

Descripción de las dos primeras líneas del archivo de entrada (Ejemplo):

Primera línea:  1  3  2

Esta primera línea indica el orden en el cual serán cargados los ferrys al inicio de la simulación.
En el ejemplo se inicia con el primer ferry (Lilia Concepción), luego el tercero (La Margariteña) y
por último el segundo ferry (La Isabela)

Segunda línea:  121  2  1  0  1  1800  800  CD345CC 0

Se refiere a un vehículo con la siguiente descripción:

- Vehículo liviano, procedente de un estado fuera de la región, con pasajeros → 121
- Traslada 2 pasajeros adultos (adicional al chofer) → 2
- Traslada 1 adulto de la tercera edad  → 1
- Los adultos viajan en VIP → 0
- La persona de tercera edad viaja en clase ejecutiva  → 1
- El peso del vehículo es de 1800 Kg. → 1800
- Hora de llegada, 8:00 a.m. → 800
- Vehículo con placa CD345CC → CD345CC
- Viaja en el ferry tipo Express → 0


Como toda estrategia bien ejecutada, cada segundo cuenta.
"La sabiduría comienza en la reflexión. Nada es verdad, todo está permitido." —
## E. A., AC II