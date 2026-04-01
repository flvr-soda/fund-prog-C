#include "ferry_sim.h"
#include <assert.h> 

// Inicializa una cola de vehículos dejándola vacía.
 
void cola_init(ColaVehiculo* q) {
    q->head = q->tail = q->size = 0;
}

 // Retorna 1 si la cola está vacía, 0 si tiene elementos.
 
int cola_vacia(const ColaVehiculo* q) {
    return q->size == 0;
}

//  Inserta el índice de un vehículo al final de la cola.
//  La cola debe tener espacio suficiente (no se debe exceder MAX_VEHICULOS).
 
void cola_push(ColaVehiculo* q, int idx) {
    assert(q->size < MAX_VEHICULOS);            // Asegura que la cola no se desborde
    q->idx[q->tail] = idx;                      // Inserta el índice en la posición tail
    q->tail = (q->tail + 1) % MAX_VEHICULOS;    // Actualiza tail circularmente
    q->size++;                                  // Incrementa el tamaño de la cola
}

 // Extrae y retorna el índice del vehículo al frente de la cola.
 // La cola no debe estar vacía.
 
int cola_pop(ColaVehiculo* q) {
    assert(q->size > 0);                     // Asegura que hay elementos para extraer
    int r = q->idx[q->head];                 // Obtiene el elemento en la posición head
    q->head = (q->head + 1) % MAX_VEHICULOS; // Actualiza head circularmente
    q->size--;                               // Decrementa el tamaño de la cola
    return r;                                // Retorna el índice extraído
}