#include "ferry_sim.h"
#include <stdio.h>
#include <string.h>

int main() {
    Vehiculo lista[MAX_VEHICULOS];
    char orden_ferry[3];
    int nvehiculos = 0;
    ColaVehiculo cola_express, cola_tradicional;
    cola_init(&cola_express);
    cola_init(&cola_tradicional);

    if (leer_vehiculos(lista, &nvehiculos, orden_ferry, "proy1.in") != 0) {
        printf("Error leyendo datos entrada\n");
        return 1;
    }

    // Inicializa ferrys, colas, estadísticas...

    int minuto_actual= /*inicio primer vehículo*/;
    while (/* Simulación no termina */) {
        // 1. Mueve vehículos que llegan en este minuto a la cola (perteneciente)
        // 2. Si ferry actual está en carga, realiza carga de un vehículo si hay espacio,
        //    respetando prioridad a vehículos emergencia
        // 3. Cuando un ferry está listo para zarpar, simula viaje y descarga, recolectando datos
        // 4. Cuando todos los ferrys y colas están vacías o no pueden cumplir mínimo, termina
        minuto_actual++;
    }

    // Imprimir reportes
    return 0;
}