/* 03/04/2026 | Ismael Armada  31.390.408  |  Rances Colotto 30.997.818  |  Sec. 08 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/* ── Dimensiones ──────────────────────────────────────────────────────────── */
#define MAX_VEH   500
#define N_FERRYS  3
#define T_CARGA   3          /* minutos por vehiculo en carga */

/* ── Tarifas pasajeros  (A=adulto  S=3ra edad  T=tradicional  E=express) ─── */
#define TP_1RA_A_T   370.00
#define TP_1RA_S_T   190.50
#define TP_TUR_A_T   290.00
#define TP_TUR_S_T   150.50
#define TP_VIP_A_E  1020.00
#define TP_VIP_S_E   520.00
#define TP_EJE_A_E   620.00
#define TP_EJE_S_E   320.00

/* ── Tarifas vehiculos ────────────────────────────────────────────────────── */
#define TV_LIV_T   590.00
#define TV_RUS_T   710.00
#define TV_MIC_T   850.00
#define TV_CAR_T  1200.00
#define TV_LIV_E  1090.00
#define TV_RUS_E  1310.00
#define TV_MIC_E  1600.00
#define TV_CAR_E  2400.00

/* ── Tipos ────────────────────────────────────────────────────────────────── */
typedef enum { EXPRESO = 0, TRADICIONAL = 1 } TipoFerry;
typedef enum { CARGANDO = 1, EN_VIAJE = 2, EN_ESPERA = 3 } Estado;

/* Vehiculo: nombres de campos segun requerimientos.md */
typedef struct {
    int  cod, npa, npt, tpa, tpt, peso, hora;
    char placa[16];
    int  tipo_ferry;
} Veh;

/* Cola circular de indices */
typedef struct {
    int buf[MAX_VEH], head, tail, size;
} Cola;

typedef struct {
    char      nombre[32];
    TipoFerry tipo;
    int       id;
    /* capacidades fijas */
    int       cap_veh, min_veh, cap_pas, cap_vip, cap_eje;
    float     cap_ton;
    int       t_ciclo;
    /* estado dinamico (se reinicia tras cada zarpe) */
    int       abordo[MAX_VEH];
    int       n_veh, pas, pas_3ra, vip_us, eje_us;
    float     ton_car;
    Estado    estado;
    int       t_rest, n_viajes;
    float     ingresos;
} Ferry;

typedef struct {
    int   tot_veh, tot_pas, pas_perd;
    float tot_ing;
    int   frec[8];           /* frecuencia por tipo de vehiculo (indices 1..7) */
    int   max_cola, h_cola;  /* mayor espera y hora militar en que ocurrio */
} Stats;

/* ── Operaciones de cola ──────────────────────────────────────────────────── */
void q_ini(Cola *q)        { q->head = q->tail = q->size = 0; }
int  q_vacia(const Cola *q){ return q->size == 0; }
int  q_ver(const Cola *q)  { assert(q->size > 0); return q->buf[q->head]; }

void q_push(Cola *q, int v) {
    assert(q->size < MAX_VEH);
    q->buf[q->tail] = v;
    q->tail = (q->tail + 1) % MAX_VEH;
    q->size++;
}
int q_pop(Cola *q) {
    int r;
    assert(q->size > 0);
    r = q->buf[q->head];
    q->head = (q->head + 1) % MAX_VEH;
    q->size--;
    return r;
}
/* Inserta al frente (vehiculos bajados del ferry con prioridad) */
void q_push_f(Cola *q, int v) {
    assert(q->size < MAX_VEH);
    q->head = (q->head - 1 + MAX_VEH) % MAX_VEH;
    q->buf[q->head] = v;
    q->size++;
}

/* ── Lectura ──────────────────────────────────────────────────────────────── */
int leer_datos(Veh lista[], int *n, int orden[3]) {
    /* Lee el archivo `proy1.in`.
     * Formato:
     *  - Primera línea: orden de inicio de carga de ferrys (1..3)
     *  - Luego, una línea por vehículo:
     *      cod npa npt tpa tpt P Hr ID f
     * Donde `lista[]` guarda los vehículos tal como vienen (respetando el
     * orden por hora de llegada, según el enunciado).
     */
    FILE *f = fopen("proy1.in", "r");
    char  lineaEntrada[128];
    int   vehiculoIdx;

    if (!f) return -1;
    if (!fgets(lineaEntrada, sizeof(lineaEntrada), f)) { fclose(f); return -2; }
    sscanf(lineaEntrada, "%d %d %d", &orden[0], &orden[1], &orden[2]);

    for (vehiculoIdx = 0;
         vehiculoIdx < MAX_VEH && fgets(lineaEntrada, sizeof(lineaEntrada), f); ) {
        Veh vehiculo;
        /* `sscanf` devuelve la cantidad de campos parseados. Esperamos 9. */
        int camposLeidos = sscanf(lineaEntrada, "%d %d %d %d %d %d %d %s %d",
            &vehiculo.cod, &vehiculo.npa, &vehiculo.npt, &vehiculo.tpa, &vehiculo.tpt,
            &vehiculo.peso, &vehiculo.hora, vehiculo.placa, &vehiculo.tipo_ferry);
        if (camposLeidos == 9) lista[vehiculoIdx++] = vehiculo;
    }

    *n = vehiculoIdx;
    fclose(f);
    return 0;
}

/* ── Inicializacion de la flota ───────────────────────────────────────────── */
void init_flota(Ferry f[]) {
    int ferryIdx;
    /* Lilia Concepcion — Express */
    strcpy(f[0].nombre, "Lilia Concepcion");
    f[0].tipo=EXPRESO;     f[0].id=1;
    f[0].cap_veh=16;       f[0].min_veh=5;   /* ceil(16*0.3)=5 */
    f[0].cap_pas=50;       f[0].cap_vip=20;  f[0].cap_eje=30;
    f[0].cap_ton=60.0f;    f[0].t_ciclo=35;

    /* La Isabela — Tradicional */
    strcpy(f[1].nombre, "La Isabela");
    f[1].tipo=TRADICIONAL; f[1].id=2;
    f[1].cap_veh=20;       f[1].min_veh=6;
    f[1].cap_pas=70;       f[1].cap_vip=20;  f[1].cap_eje=50;
    f[1].cap_ton=80.0f;    f[1].t_ciclo=65;

    /* La Margaritena — Tradicional */
    strcpy(f[2].nombre, "La Margaritena");
    f[2].tipo=TRADICIONAL; f[2].id=3;
    f[2].cap_veh=18;       f[2].min_veh=6;   /* ceil(18*0.3)=6 */
    f[2].cap_pas=60;       f[2].cap_vip=20;  f[2].cap_eje=40;
    f[2].cap_ton=80.0f;    f[2].t_ciclo=65;

    /* Reinicia estado dinamico */
    for (ferryIdx = 0; ferryIdx < N_FERRYS; ferryIdx++) {
        f[ferryIdx].n_veh=0; f[ferryIdx].pas=0; f[ferryIdx].pas_3ra=0;
        f[ferryIdx].vip_us=0; f[ferryIdx].eje_us=0; f[ferryIdx].ton_car=0.0f;
        f[ferryIdx].estado=EN_ESPERA; f[ferryIdx].t_rest=0;
        f[ferryIdx].n_viajes=0; f[ferryIdx].ingresos=0.0f;
    }
}

/* ── Funciones auxiliares ─────────────────────────────────────────────────── */
int   tipo_v(const Veh *v)   { return v->cod / 100; }
int   lleva_pas(const Veh *v){ return v->cod % 10; }
int   es_emerg(const Veh *v) {
    /* Emergencias: ambulancia (5), bomberos (6) y policial (7). */
    int tipoVehiculo = tipo_v(v);
    return tipoVehiculo==5||tipoVehiculo==6||tipoVehiculo==7;
}
float v_ton(const Veh *v)    { return tipo_v(v)==4 ? (float)v->peso : v->peso/1000.0f; }
int   h_a_min(int hm)        { return (hm/100)*60 + hm%60; }
int   min_a_h(int m)         { return ((m/60)%24)*100 + m%60; }

const char *nom_tipo(int t) {
    switch (t) {
        case 1: return "liviano";    case 2: return "rustico";
        case 3: return "microbus";   case 4: return "carga";
        case 5: return "ambulancia"; case 6: return "bomberos";
        case 7: return "policial";   default: return "?";
    }
}

/* Ingreso por el vehiculo (tarifa segun tipo y ferry) */
float ing_veh(const Veh *v, TipoFerry tipoFerry) {
    int tipoVehiculo = tipo_v(v);
    if (tipoFerry == EXPRESO) {
        if (tipoVehiculo==1||tipoVehiculo==5||tipoVehiculo==6||tipoVehiculo==7) return TV_LIV_E;
        if (tipoVehiculo==2) return TV_RUS_E;
        if (tipoVehiculo==3) return TV_MIC_E;
        if (tipoVehiculo==4) return TV_CAR_E;
    } else {
        if (tipoVehiculo==1||tipoVehiculo==5||tipoVehiculo==6||tipoVehiculo==7) return TV_LIV_T;
        if (tipoVehiculo==2) return TV_RUS_T;
        if (tipoVehiculo==3) return TV_MIC_T;
        if (tipoVehiculo==4) return TV_CAR_T;
    }
    return 0.0f;
}

/* Ingreso por pasajeros (incluye el chofer) */
float ing_pas(const Veh *v, TipoFerry tipoFerry) {
    float ingresoPasajeros = 0.0f;
    if (tipoFerry == EXPRESO) {
        ingresoPasajeros += (v->tpa==0) ? TP_VIP_A_E : TP_EJE_A_E;        /* chofer (adulto) */
        if (lleva_pas(v)) {
            ingresoPasajeros += v->npa * ((v->tpa==0) ? TP_VIP_A_E : TP_EJE_A_E);
            ingresoPasajeros += v->npt * ((v->tpt==0) ? TP_VIP_S_E : TP_EJE_S_E);
        }
    } else {
        ingresoPasajeros += (v->tpa==0) ? TP_1RA_A_T : TP_TUR_A_T;         /* chofer (adulto) */
        if (lleva_pas(v)) {
            ingresoPasajeros += v->npa * ((v->tpa==0) ? TP_1RA_A_T : TP_TUR_A_T);
            ingresoPasajeros += v->npt * ((v->tpt==0) ? TP_1RA_S_T : TP_TUR_S_T);
        }
    }
    return ingresoPasajeros;
}

/* ── Logica de embarque ───────────────────────────────────────────────────── */

/* Devuelve 1 si v cabe en el ferry (vehiculos, pasajeros, asientos, peso) */
int puede_subir(const Ferry *f, const Veh *v) {
    int llevaPasajeros = lleva_pas(v);

    /* El chofer SIEMPRE cuenta como pasajero adulto (1 asiento adulto). */
    int pasajerosTotalesNuevos = 1 + (llevaPasajeros ? v->npa + v->npt : 0);

    /* Asientos por clase: VIP (tpa/tpt==0) y Ejecutiva/Turista (tpa/tpt==1). */
    int vipNuevos = (v->tpa==0 ? 1 : 0) + (llevaPasajeros && v->tpa==0 ? v->npa : 0)
                                          + (llevaPasajeros && v->tpt==0 ? v->npt : 0);
    int ejeNuevos = (v->tpa==1 ? 1 : 0) + (llevaPasajeros && v->tpa==1 ? v->npa : 0)
                                          + (llevaPasajeros && v->tpt==1 ? v->npt : 0);

    /* Capacidad por vehiculos. */
    if (f->n_veh >= f->cap_veh) return 0;
    /* Capacidad por total de pasajeros. */
    if (f->pas + pasajerosTotalesNuevos > f->cap_pas) return 0;
    /* Capacidad por clase VIP. */
    if (f->vip_us + vipNuevos > f->cap_vip) return 0;
    /* Capacidad por clase Ejecutiva/Turista. */
    if (f->eje_us + ejeNuevos > f->cap_eje) return 0;
    /* Capacidad por peso total (toneladas). */
    if (f->ton_car + v_ton(v) > f->cap_ton) return 0;

    return 1;
}

/* Embarca el vehiculo idx, actualizando contadores del ferry */
void embarcar(Ferry *f, Veh lista[], int idx) {
    const Veh *vehiculo = &lista[idx];
    int llevaPasajeros = lleva_pas(vehiculo);

    f->abordo[f->n_veh++] = idx;
    f->ton_car += v_ton(vehiculo);
    f->pas++;
    if (vehiculo->tpa==0) f->vip_us++; else f->eje_us++;

    if (llevaPasajeros) {
        f->pas    += vehiculo->npa + vehiculo->npt;
        f->pas_3ra += vehiculo->npt;
        if (vehiculo->tpa==0) f->vip_us += vehiculo->npa; else f->eje_us += vehiculo->npa;
        if (vehiculo->tpt==0) f->vip_us += vehiculo->npt; else f->eje_us += vehiculo->npt;
    }
}

/* Revierte los contadores de un vehiculo (para bajar del ferry) */
void desembarcar_cnt(Ferry *f, const Veh *v) {
    int llevaPasajeros = lleva_pas(v);

    f->ton_car -= v_ton(v);
    f->pas--;
    if (v->tpa==0) f->vip_us--; else f->eje_us--;

    if (llevaPasajeros) {
        f->pas    -= v->npa + v->npt;
        f->pas_3ra -= v->npt;
        if (v->tpa==0) f->vip_us -= v->npa; else f->eje_us -= v->npa;
        if (v->tpt==0) f->vip_us -= v->npt; else f->eje_us -= v->npt;
    }
}

/* Devuelve 1 si el ferry puede zarpar segun las condiciones del enunciado:
 *  a) Lleno en vehiculos O lleno en pasajeros.
 *  b) Alcanzo minimo de vehiculos y la cola esta vacia.
 *  c) Alcanzo minimo y ningun vehiculo en cola puede abordar
 *     (por peso, pasajeros, asientos o espacio). */
int puede_zarpar(const Ferry *f, const Cola *cola, Veh lista[]) {
    int idxVehiculoCola;

    /* Condicion (a): lleno por vehiculos o por pasajeros. */
    if (f->n_veh >= f->cap_veh) return 1;
    if (f->pas   >= f->cap_pas) return 1;

    /* Si no alcanza el mínimo de vehiculos, no puede zarpar. */
    if (f->n_veh < f->min_veh)  return 0;

    /* Condicion (b): mínimo OK + cola vacía. */
    if (q_vacia(cola))          return 1;

    /* Condicion (c): mínimo OK y ningún vehiculo de la cola puede abordar. */
    /* Recorre la cola: si existe algún vehiculo que pueda abordar,
     * entonces el ferry NO puede zarpar. */
    for (idxVehiculoCola = 0; idxVehiculoCola < cola->size; idxVehiculoCola++) {
        int posicionBuffer = (cola->head + idxVehiculoCola) % MAX_VEH;
        if (puede_subir(f, &lista[cola->buf[posicionBuffer]])) {
            return 0; /* al menos uno puede subir, no zarpa aun */
        }
    }
    return 1;
}

/* ── Reporte ──────────────────────────────────────────────────────────────── */
void rep_viaje(const Ferry *f, Veh lista[], int numeroCarga, FILE *out) {
    /* Calcula el ingreso total del viaje y reporta:
     *  - ferry
     *  - Nro de viaje y cantidad de vehiculos/pasajeros
     *  - peso e ingreso
     *  - lista de placas y tipo de vehiculos abordo
     * El reporte se muestra en pantalla y se guarda en `proy1.out`.
     */
    int   idxVehiculoAbordo;
    float ingresoViaje = 0.0f;
    FILE *destinos[2] = { stdout, out };
    int idxDestino;

    for (idxVehiculoAbordo = 0; idxVehiculoAbordo < f->n_veh; idxVehiculoAbordo++) {
        const Veh *vehiculoAbordo = &lista[f->abordo[idxVehiculoAbordo]];
        ingresoViaje += ing_veh(vehiculoAbordo, f->tipo) + ing_pas(vehiculoAbordo, f->tipo);
    }

    /* Imprime en pantalla (stdout) y archivo (out). */
    for (idxDestino = 0; idxDestino < 2; idxDestino++) {
        if (!destinos[idxDestino]) continue;

        fprintf(destinos[idxDestino], "Carga Nro. %d:\n", numeroCarga);
        fprintf(destinos[idxDestino], "Ferry %s\n", f->nombre);
        fprintf(destinos[idxDestino], "Viaje Nro. %d\n", f->n_viajes);
        fprintf(destinos[idxDestino], "Num. vehiculos: %d\n", f->n_veh);
        fprintf(destinos[idxDestino], "Num. pasajeros: %d (%d mayores de 60 anos)\n",
                f->pas, f->pas_3ra);
        fprintf(destinos[idxDestino], "Peso: %.2f toneladas\n", f->ton_car);
        fprintf(destinos[idxDestino], "Ingreso: %.0f BsF.\n", ingresoViaje);

        for (idxVehiculoAbordo = 0; idxVehiculoAbordo < f->n_veh; idxVehiculoAbordo++) {
            const Veh *vehiculoAbordo = &lista[f->abordo[idxVehiculoAbordo]];
            fprintf(destinos[idxDestino], "ID: %-12s Tipo: %s\n",
                    vehiculoAbordo->placa, nom_tipo(tipo_v(vehiculoAbordo)));
        }

        fprintf(destinos[idxDestino], "\n");
    }
}

void rep_stats(const Stats *s, FILE *out) {
    /* Resume las estadísticas al final del día. */
    int indiceTipoMasFrecuente = 1;
    FILE *destinos[2] = { stdout, out };
    int tipoIdx;
    int idxDestino;

    /* s->frec[t] acumula la frecuencia por tipo (t en 1..7). */
    for (tipoIdx = 2; tipoIdx <= 7; tipoIdx++) {
        if (s->frec[tipoIdx] > s->frec[indiceTipoMasFrecuente]) {
            indiceTipoMasFrecuente = tipoIdx;
        }
    }
    for (idxDestino = 0; idxDestino < 2; idxDestino++) {
        if (!destinos[idxDestino]) continue;

        fprintf(destinos[idxDestino], "Estadisticas:\n");
        fprintf(destinos[idxDestino], "Total vehiculos transportados: %d\n",    s->tot_veh);
        fprintf(destinos[idxDestino], "Total pasajeros transportados: %d\n",    s->tot_pas);
        fprintf(destinos[idxDestino], "Total de ingresos: %.0f BsF.\n",         s->tot_ing);
        fprintf(destinos[idxDestino], "Num. pasajeros no trasladados: %d\n",    s->pas_perd);
        fprintf(destinos[idxDestino], "Vehiculo mas frecuente: %s\n",
                nom_tipo(indiceTipoMasFrecuente));
        fprintf(destinos[idxDestino], "Mayor frec. vehiculos en espera: %d (a las %04d)\n",
                s->max_cola, s->h_cola);
    }
}

/* ── Simulacion ───────────────────────────────────────────────────────────── */
void simular(Veh lista[], int n, int orden[3], Ferry flota[], FILE *out) {
    /* Cola de espera para ferry Express (cola normal por tipo Express). */
    Cola colaExpress;
    /* Cola de espera para ferry Tradicional (cola normal por tipo Tradicional). */
    Cola colaTradicional;
    /* Cola de emergencias (ambulancias, bomberos y policial). */
    Cola colaEmergencias;
    /* Cola circular con los ferrys que están regresando (en espera de cargar). */
    Cola colaMuelle;

    Stats statsDia;

    int vehiculoIdxProximo;
    int minutoActual;
    int minutoFin;
    int ferryEnMuelle;
    int minutoDisponibleCarga;
    int numeroCarga;
    int ferryIdx;
    int idx;

    /* Inicializa colas y estadísticas. */
    q_ini(&colaExpress);
    q_ini(&colaTradicional);
    q_ini(&colaEmergencias);
    q_ini(&colaMuelle);

    statsDia.tot_veh = 0;
    statsDia.tot_pas = 0;
    statsDia.pas_perd = 0;
    statsDia.tot_ing = 0.0f;
    statsDia.max_cola = 0;
    statsDia.h_cola = 0;
    for (idx = 0; idx < 8; idx++) statsDia.frec[idx] = 0;

    /* Cola de muelle según orden de inicio del archivo (IDs 1,2,3 → indices 0,1,2) */
    for (idx = 0; idx < N_FERRYS; idx++) q_push(&colaMuelle, orden[idx] - 1);

    minutoActual = h_a_min(lista[0].hora);
    minutoFin = minutoActual + 1440;
    vehiculoIdxProximo = 0;
    numeroCarga = 0;

    ferryEnMuelle = q_pop(&colaMuelle);
    flota[ferryEnMuelle].estado = CARGANDO;
    minutoDisponibleCarga = minutoActual;

    while (minutoActual <= minutoFin) {
        /* 1) Encolar vehículos que llegan exactamente en este minuto. */
        while (vehiculoIdxProximo < n &&
               h_a_min(lista[vehiculoIdxProximo].hora) == minutoActual) {
            int vehiculoIdxLlegada = vehiculoIdxProximo++;
            /* Contar TODOS los vehículos del día. */
            statsDia.frec[tipo_v(&lista[vehiculoIdxLlegada])]++;

            if (es_emerg(&lista[vehiculoIdxLlegada])) {
                q_push(&colaEmergencias, vehiculoIdxLlegada);
            } else if (lista[vehiculoIdxLlegada].tipo_ferry == 0) {
                q_push(&colaExpress, vehiculoIdxLlegada);
            } else {
                q_push(&colaTradicional, vehiculoIdxLlegada);
            }
        }

        /* 2) Carga del ferry que está en el muelle (si ya pasó el tiempo de carga). */
        if (ferryEnMuelle >= 0 && minutoActual >= minutoDisponibleCarga) {
            Ferry *ferryActual = &flota[ferryEnMuelle];
            Cola  *colaFerry = (ferryActual->tipo == EXPRESO) ? &colaExpress : &colaTradicional;

            /* Prioridad 1: emergencias */
            if (!q_vacia(&colaEmergencias)) {
                int vehiculoEmergenciaIdx = q_pop(&colaEmergencias);

                /* Si el ferry está lleno, bajar el último vehículo normal para dejar entrar la emergencia. */
                while (ferryActual->n_veh >= ferryActual->cap_veh) {
                    int vehiculoRetiradoIdx = ferryActual->abordo[--ferryActual->n_veh];
                    Cola *colaDestino = (lista[vehiculoRetiradoIdx].tipo_ferry == 0)
                        ? &colaExpress : &colaTradicional;
                    desembarcar_cnt(ferryActual, &lista[vehiculoRetiradoIdx]);
                    q_push_f(colaDestino, vehiculoRetiradoIdx);
                }

                embarcar(ferryActual, lista, vehiculoEmergenciaIdx);
                minutoDisponibleCarga = minutoActual + T_CARGA;

            /* Prioridad 2: cola normal del ferry */
            } else if (!q_vacia(colaFerry)) {
                int vehiculoFrenteIdx = q_ver(colaFerry);
                if (puede_subir(ferryActual, &lista[vehiculoFrenteIdx])) {
                    q_pop(colaFerry);
                    embarcar(ferryActual, lista, vehiculoFrenteIdx);
                    minutoDisponibleCarga = minutoActual + T_CARGA;
                }
            }

            /* Verifica zarpe (no zarpa si hay emergencias esperando). */
            if (q_vacia(&colaEmergencias) && puede_zarpar(ferryActual, colaFerry, lista)) {
                /* Acumula estadísticas del viaje */
                for (ferryIdx = 0; ferryIdx < ferryActual->n_veh; ferryIdx++) {
                    Veh *vehiculoAbordo = &lista[ferryActual->abordo[ferryIdx]];
                    statsDia.tot_veh++;
                    statsDia.frec[tipo_v(vehiculoAbordo)]++;
                    statsDia.tot_ing += ing_veh(vehiculoAbordo, ferryActual->tipo)
                                      + ing_pas(vehiculoAbordo, ferryActual->tipo);
                    statsDia.tot_pas += 1 + (lleva_pas(vehiculoAbordo)
                                             ? vehiculoAbordo->npa + vehiculoAbordo->npt
                                             : 0);
                }

                ferryActual->n_viajes++;
                numeroCarga++;
                rep_viaje(ferryActual, lista, numeroCarga, out);

                /* Reinicia ferry y lo pone en viaje */
                ferryActual->estado = EN_VIAJE;
                ferryActual->t_rest = ferryActual->t_ciclo;
                ferryActual->n_veh = 0;
                ferryActual->pas = 0;
                ferryActual->pas_3ra = 0;
                ferryActual->vip_us = 0;
                ferryActual->eje_us = 0;
                ferryActual->ton_car = 0.0f;

                /* Siguiente ferry al muelle */
                ferryEnMuelle = q_vacia(&colaMuelle) ? -1 : q_pop(&colaMuelle);
                if (ferryEnMuelle >= 0) {
                    flota[ferryEnMuelle].estado = CARGANDO;
                    minutoDisponibleCarga = minutoActual;
                }
            }
        } else if (ferryEnMuelle < 0 && !q_vacia(&colaMuelle)) {
            /* Si el muelle está vacío, toma el siguiente ferry disponible. */
            ferryEnMuelle = q_pop(&colaMuelle);
            flota[ferryEnMuelle].estado = CARGANDO;
            minutoDisponibleCarga = minutoActual;
        }

        /* 3) Estadísticas de espera (se mide DESPUÉS de cargar del minuto actual). */
        {
            int cantidadEnEspera = colaExpress.size + colaTradicional.size + colaEmergencias.size;
            if (cantidadEnEspera > statsDia.max_cola) {
                statsDia.max_cola = cantidadEnEspera;
                statsDia.h_cola = min_a_h(minutoActual);
            }
        }

        /* 4) Avanza ferrys que están en viaje. */
        for (ferryIdx = 0; ferryIdx < N_FERRYS; ferryIdx++) {
            if (flota[ferryIdx].estado == EN_VIAJE && --flota[ferryIdx].t_rest <= 0) {
                flota[ferryIdx].estado = EN_ESPERA;
                q_push(&colaMuelle, ferryIdx);
            }
        }

        /* 5) Condición de fin:
         * cuando no quedan vehículos por entrar a las colas y no hay ferrys que puedan zarpar. */
        if (vehiculoIdxProximo >= n &&
            colaExpress.size == 0 && colaTradicional.size == 0 && colaEmergencias.size == 0) {
            int hayFerryListo = (ferryEnMuelle >= 0 &&
                                  flota[ferryEnMuelle].n_veh >= flota[ferryEnMuelle].min_veh);
            if (!hayFerryListo) break;
        }

        minutoActual++;
    }

    /* Pasajeros no trasladados: en colas de espera que quedaron con vehículos al finalizar. */
    {
        Cola *colasSobrantes[3] = { &colaEmergencias, &colaExpress, &colaTradicional };
        int tipoCola;
        for (tipoCola = 0; tipoCola < 3; tipoCola++) {
            for (idx = 0; idx < colasSobrantes[tipoCola]->size; idx++) {
                int pos = (colasSobrantes[tipoCola]->head + idx) % MAX_VEH;
                Veh *vehiculoPendiente = &lista[colasSobrantes[tipoCola]->buf[pos]];
                statsDia.pas_perd += 1 + (lleva_pas(vehiculoPendiente)
                                           ? vehiculoPendiente->npa + vehiculoPendiente->npt
                                           : 0);
            }
        }
    }

    /* Pasajeros dentro del ferry que quedó en el muelle pero que no alcanzó el mínimo para zarpar. */
    if (ferryEnMuelle >= 0 && flota[ferryEnMuelle].n_veh > 0) {
        Ferry *ferryFinalMuelle = &flota[ferryEnMuelle];
        for (idx = 0; idx < ferryFinalMuelle->n_veh; idx++) {
            const Veh *vehiculoAbordo = &lista[ferryFinalMuelle->abordo[idx]];
            statsDia.pas_perd += 1 + (lleva_pas(vehiculoAbordo)
                                      ? vehiculoAbordo->npa + vehiculoAbordo->npt
                                      : 0);
        }
    }

    rep_stats(&statsDia, out);
}

/* ── Main ─────────────────────────────────────────────────────────────────── */
int main(void) {
    Veh   lista[MAX_VEH];
    Ferry flota[N_FERRYS];
    int   orden[3], n = 0;
    FILE *out;

    if (leer_datos(lista, &n, orden) != 0) {
        printf("Error: no se pudo abrir proy1.in\n");
        return 1;
    }

    init_flota(flota);
    out = fopen("proy1.out", "w");
    simular(lista, n, orden, flota, out);
    if (out) fclose(out);
    return 0;
}