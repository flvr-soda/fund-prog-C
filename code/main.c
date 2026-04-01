// 03/04/2026 Sección-08
// Ismael Armada 31.390.408 
// Rances Colotto 30.997.818
#include "ferry_sim.h"
#include "lectura.h"
#include "colas.c"
#include <stdio.h>
#include <string.h>

int main() {
    Vehiculo lista[MAX_VEHICULOS];
    int orden_ferry[3];
    int nvehiculos = 0;
    Ferry flota[MAX_FERRYS];
    ColaVehiculo cola_express, cola_tradicional;
    cola_init(&cola_express);
    cola_init(&cola_tradicional);

    if (leer_vehiculos(lista, &nvehiculos, orden_ferry) != 0) {
        printf("Error leyendo datos entrada\n");
        return 1;
    }

    // Inicializa ferrys, colas, estadísticas...
    inicializar_ferrys(flota);

    int minuto_actual = 0; /*inicio primer vehículo*/
    while (1 /* Simulación no termina */) {
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

void inicializar_ferrys(Ferry flota[]) {
    // 0: Lilia Concepción (Express)
    strcpy(flota[0].nombre, "Lilia Concepcion");
    flota[0].tipo = FERRY_EXPRESS;
    flota[0].id = 1;
    flota[0].max_vehiculos = 16;
    flota[0].min_vehiculos = 5; // 30% de 16
    flota[0].max_pasajeros = 50;
    flota[0].max_vip_1ra = 20;
    flota[0].max_ejecutiva = 30;
    flota[0].max_peso_ton = 60;
    flota[0].t_ciclo = 35;
    flota[0].estado = 3;
    flota[0].num_vehiculos = 0;
    flota[0].pasajeros = 0;
    flota[0].pasajeros_tercera_edad = 0;
    flota[0].peso_cargado = 0;

    // 1: La Isabela (Tradicional)
    strcpy(flota[1].nombre, "La Isabela");
    flota[1].tipo = FERRY_TRADICIONAL;
    flota[1].id = 2;
    flota[1].max_vehiculos = 20;
    flota[1].min_vehiculos = 6; // 30% de 20
    flota[1].max_pasajeros = 70;
    flota[1].max_vip_1ra = 20;
    flota[1].max_ejecutiva = 50; 
    flota[1].max_peso_ton = 80;
    flota[1].t_ciclo = 65;
    flota[1].estado = 3;
    flota[1].num_vehiculos = 0;
    flota[1].pasajeros = 0;
    flota[1].pasajeros_tercera_edad = 0;
    flota[1].peso_cargado = 0;

    // 2: La Margariteña (Tradicional)
    strcpy(flota[2].nombre, "La Margaritena");
    flota[2].tipo = FERRY_TRADICIONAL;
    flota[2].id = 3;
    flota[2].max_vehiculos = 18;
    flota[2].min_vehiculos = 6; // 30% de 18
    flota[2].max_pasajeros = 60;
    flota[2].max_vip_1ra = 20;
    flota[2].max_ejecutiva = 40; 
    flota[2].max_peso_ton = 80;
    flota[2].t_ciclo = 65;
    flota[2].estado = 3;
    flota[2].num_vehiculos = 0;
    flota[2].pasajeros = 0;
    flota[2].pasajeros_tercera_edad = 0;
    flota[2].peso_cargado = 0;
}

// Rances no se que estamos haciendo pero creo que compila xd