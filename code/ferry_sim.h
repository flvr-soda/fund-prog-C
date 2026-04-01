#ifndef FERRY_SIM_H
#define FERRY_SIM_H

#define MAX_VEHICULOS 500        // Estimación razonable para un día (ajustable)
#define MAX_PASAJEROS_TOTAL 1000
#define MAX_FERRYS 3

#define TIEMPO_CARGA_DESCARGA 3

// bomberos, policias etc no tienen tarifas lo podriamos obviar o ponerles 0 
// Tarifas de Pasajeros - Ferry Tradicional (Convencional)
#define TARIFA_ADULTO_1RA_TRAD 370.00
#define TARIFA_TERCERA_1RA_TRAD 190.50
#define TARIFA_ADULTO_TUR_TRAD 290.00
#define TARIFA_TERCERA_TUR_TRAD 150.50

// Tarifas de Pasajeros - Ferry Express
#define TARIFA_ADULTO_VIP_EXP 1020.00
#define TARIFA_TERCERA_VIP_EXP 520.00
#define TARIFA_ADULTO_EJE_EXP 620.00
#define TARIFA_TERCERA_EJE_EXP 320.00

// Tarifas de Vehículos - Ferry Tradicional
#define TARIFA_VEH_LIVIANO_TRAD 590.00
#define TARIFA_VEH_RUSTICO_TRAD 710.00
#define TARIFA_VEH_MICROBUS_TRAD 850.00
#define TARIFA_VEH_CARGA_TRAD 1200.00

// Tarifas de Vehículos - Ferry Express
#define TARIFA_VEH_LIVIANO_EXP 1090.00
#define TARIFA_VEH_RUSTICO_EXP 1310.00
#define TARIFA_VEH_MICROBUS_EXP 1600.00
#define TARIFA_VEH_CARGA_EXP 2400.00

typedef enum {
    FERRY_EXPRESS = 0,
    FERRY_TRADICIONAL = 1
} FerryTipo;

typedef enum {
    LIVIANO = 1, RUSTICO, MICROBUS, CARGA,
    AMBULANCIA, BOMBEROS, POLICIAL
} TipoVehiculo;

typedef struct {
    int cod;        // Código del vehículo
    int npa;        // Pasajeros adultos
    int npt;        // Pasajeros tercera edad
    int tpa;        // Tipo pasaje adulto (0 primera/VIP, 1 turista/ejecutiva)
    int tpt;        // Tipo pasaje tercera edad
    int peso;       // Peso (kg o toneladas)
    int hora;       // Hora de llegada (ej: 800)
    char placa[16]; // Placa
    int tipo_ferry; // 0 Express, 1 Tradicional
} Vehiculo;

typedef struct {
    char nombre[32];
    FerryTipo tipo;
    int max_vehiculos;
    int min_vehiculos;          // 30% de capacidad requerido para zarpar
    int max_pasajeros;          // suma de todas las clases
    int max_vip_1ra;            // asientos VIP/1ra clase
    int max_ejecutiva;          // asientos ejecutiva/turista
    int max_peso_ton;           // Peso máximo en toneladas
    int t_ciclo;                // Minutos por viaje (ida+vuelta)
    int vehiculos[MAX_VEHICULOS];
    int num_vehiculos;          // Índice libre para el próximo
    int pasajeros;              // Pasajeros a bordo (con choferes)
    int pasajeros_tercera_edad;
    float peso_cargado;         // toneladas
    int estado;                 // 1=cargando, 2=viaje, 3=espera
    int tiempo_restante;        // Para viajes/espera/carga
    int id;                     // 1=Lilia C, 2=Isabela, 3=Margariteña
} Ferry;

typedef struct {
    int idx[MAX_VEHICULOS];
    int head, tail, size;
} ColaVehiculo;

// Prototipos de funciones
void inicializar_ferrys(Ferry flota[]);

// Prototipos de funciones de colas.c
void cola_init(ColaVehiculo* q);
int cola_vacia(const ColaVehiculo* q);
void cola_push(ColaVehiculo* q, int idx);
int cola_pop(ColaVehiculo* q);

#endif // FERRY_SIM_H
