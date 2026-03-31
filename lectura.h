#include "ferry_sim.h"
#include <stdio.h>
#include <string.h>

/*
 * Lee los datos de vehículos y el orden de ferrys desde el archivo de entrada.
 * Parámetros:
 *   lista: arreglo donde se almacenarán los vehículos leídos.
 *   n: puntero a entero donde se guardará la cantidad de vehículos leídos.
 *   orden: arreglo de tamaño 3 para guardar el orden de los ferrys.
 *   fname: nombre del archivo de entrada.
 * Retorna:
 *   0 si fue exitoso, un número negativo en caso de error.
 */
int leer_vehiculos(Vehiculo lista[], int *n, char orden[], const char* fname) {
    FILE *f = fopen(fname, "r");
    if (!f) return -1;  // No se pudo abrir el archivo
    int i = 0;
    char line[128];

    // Lee la primera línea, que indica el orden de embarque de los ferrys
    if (!fgets(line, sizeof(line), f)) return -2;
    int fo[3];
    sscanf(line, "%d %d %d", &fo[0], &fo[1], &fo[2]);
    for (int j=0; j<3; j++)
        orden[j] = fo[j];

    // Lee las siguientes líneas, cada una representa un vehículo
    while (fgets(line, sizeof(line), f)) {
        Vehiculo v;
        int c;
        // Extrae todos los datos del vehículo desde la línea
        c = sscanf(line, "%d %d %d %d %d %d %d %s %d",
            &v.cod, &v.npa, &v.npt, &v.tpa, &v.tpt, &v.peso,
            &v.hora, v.placa, &v.tipo_ferry);
        if (c < 9) break; // Si no se lograron leer todos los campos, termina
        lista[i++] = v;   // Agrega el vehículo al arreglo
    }
    *n = i; // Cantidad de vehículos leídos
    fclose(f);
    return 0;
}