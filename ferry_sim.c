#define MAX_VEHICULOS 500  // Estimación razonable para un día (ajustable)
#define MAX_PASAJEROS_TOTAL 1000
#define MAX_FERRYS 3

typedef enum {
    FERRY_EXPRESS = 0,
    FERRY_TRADICIONAL = 1
} FerryTipo;

typedef enum {
    LIVIANO = 1, RUSTICO, MICROBUS, CARGA,
    AMBULANCIA, BOMBEROS, POLICIAL
} TipoVehiculo;

typedef struct {
    int cod;       // Código del vehículo
    int npa;       // Pasajeros adultos
    int npt;       // Pasajeros tercera edad
    int tpa;       // Tipo pasaje adulto (0 primera/VIP, 1 turista/ejecutiva)
    int tpt;       // Tipo pasaje tercera edad
    int peso;      // Peso (kg o toneladas)
    int hora;      // Hora de llegada (ej: 800)
    char placa[16];// Placa
    int tipo_ferry;// 0 Express, 1 Tradicional
} Vehiculo;

typedef struct {
    char nombre[32];
    FerryTipo tipo;
    int max_vehiculos;
    int max_pasajeros;    // suma de todas las clases
    int max_vip_1ra;      // asientos VIP/1ra clase
    int max_ejecutiva;    // asientos ejecutiva/turista
    int max_peso_ton;     // Peso máximo en toneladas
    int t_ciclo;          // Minutos por viaje (ida+vuelta)
    int vehiculos[MAX_VEHICULOS];
    int num_vehiculos;    // Índice libre para el próximo
    int pasajeros;        // Pasajeros a bordo (con choferes)
    int pasajeros_tercera_edad;
    float peso_cargado;   // toneladas
    int estado;           // 1=cargando, 2=viaje, 3=espera
    int tiempo_restante;  // Para viajes/espera/carga
    int id;               // 1=Lilia C, 2=Isabela, 3=Margariteña
} Ferry;

typedef struct {
    int idx[MAX_VEHICULOS];
    int head, tail, size;
} ColaVehiculo;