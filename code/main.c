// 03/04/2026 | Ismael Armada  31.390.408  |  Rances Colotto 30.997.818  |  Sec. 08

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
    FILE *f = fopen("proy1.in", "r");
    char  ln[128];
    int   i;
    if (!f) return -1;
    if (!fgets(ln, sizeof(ln), f)) { fclose(f); return -2; }
    sscanf(ln, "%d %d %d", &orden[0], &orden[1], &orden[2]);
    for (i = 0; i < MAX_VEH && fgets(ln, sizeof(ln), f); ) {
        Veh v;
        int c = sscanf(ln, "%d %d %d %d %d %d %d %s %d",
            &v.cod, &v.npa, &v.npt, &v.tpa, &v.tpt,
            &v.peso, &v.hora, v.placa, &v.tipo_ferry);
        if (c == 9) lista[i++] = v;
    }
    *n = i;
    fclose(f);
    return 0;
}

/* ── Inicializacion de la flota ───────────────────────────────────────────── */
void init_flota(Ferry f[]) {
    int i;
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
    for (i = 0; i < N_FERRYS; i++) {
        f[i].n_veh=0; f[i].pas=0; f[i].pas_3ra=0;
        f[i].vip_us=0; f[i].eje_us=0; f[i].ton_car=0.0f;
        f[i].estado=EN_ESPERA; f[i].t_rest=0;
        f[i].n_viajes=0; f[i].ingresos=0.0f;
    }
}

/* ── Funciones auxiliares ─────────────────────────────────────────────────── */
int   tipo_v(const Veh *v)   { return v->cod / 100; }
int   lleva_pas(const Veh *v){ return v->cod % 10; }
int   es_emerg(const Veh *v) { int t=tipo_v(v); return t==5||t==6||t==7; }
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
float ing_veh(const Veh *v, TipoFerry ft) {
    int t = tipo_v(v);
    if (ft == EXPRESO) {
        if (t==1||t==5||t==6||t==7) return TV_LIV_E;
        if (t==2) return TV_RUS_E;
        if (t==3) return TV_MIC_E;
        if (t==4) return TV_CAR_E;
    } else {
        if (t==1||t==5||t==6||t==7) return TV_LIV_T;
        if (t==2) return TV_RUS_T;
        if (t==3) return TV_MIC_T;
        if (t==4) return TV_CAR_T;
    }
    return 0.0f;
}

/* Ingreso por pasajeros (incluye el chofer) */
float ing_pas(const Veh *v, TipoFerry ft) {
    float total = 0.0f;
    if (ft == EXPRESO) {
        total += (v->tpa==0) ? TP_VIP_A_E : TP_EJE_A_E;        /* chofer */
        if (lleva_pas(v)) {
            total += v->npa * ((v->tpa==0) ? TP_VIP_A_E : TP_EJE_A_E);
            total += v->npt * ((v->tpt==0) ? TP_VIP_S_E : TP_EJE_S_E);
        }
    } else {
        total += (v->tpa==0) ? TP_1RA_A_T : TP_TUR_A_T;         /* chofer */
        if (lleva_pas(v)) {
            total += v->npa * ((v->tpa==0) ? TP_1RA_A_T : TP_TUR_A_T);
            total += v->npt * ((v->tpt==0) ? TP_1RA_S_T : TP_TUR_S_T);
        }
    }
    return total;
}

/* ── Logica de embarque ───────────────────────────────────────────────────── */

/* Devuelve 1 si v cabe en el ferry (vehiculos, pasajeros, asientos, peso) */
int puede_subir(const Ferry *f, const Veh *v) {
    int lp = lleva_pas(v);
    int n_pas_new = 1 + (lp ? v->npa + v->npt : 0);   /* 1=chofer */
    int n_vip_new = (v->tpa==0 ? 1 : 0) + (lp && v->tpa==0 ? v->npa : 0)
                                          + (lp && v->tpt==0 ? v->npt : 0);
    int n_eje_new = (v->tpa==1 ? 1 : 0) + (lp && v->tpa==1 ? v->npa : 0)
                                          + (lp && v->tpt==1 ? v->npt : 0);
    if (f->n_veh   >= f->cap_veh)                          return 0;
    if (f->pas     + n_pas_new > f->cap_pas)               return 0;
    if (f->vip_us  + n_vip_new > f->cap_vip)               return 0;
    if (f->eje_us  + n_eje_new > f->cap_eje)               return 0;
    if (f->ton_car + v_ton(v)  > f->cap_ton)               return 0;
    return 1;
}

/* Embarca el vehiculo idx, actualizando contadores del ferry */
void embarcar(Ferry *f, Veh lista[], int idx) {
    const Veh *v = &lista[idx];
    int lp = lleva_pas(v);
    f->abordo[f->n_veh++] = idx;
    f->ton_car += v_ton(v);
    f->pas++;
    if (v->tpa==0) f->vip_us++; else f->eje_us++;
    if (lp) {
        f->pas    += v->npa + v->npt;
        f->pas_3ra += v->npt;
        if (v->tpa==0) f->vip_us += v->npa; else f->eje_us += v->npa;
        if (v->tpt==0) f->vip_us += v->npt; else f->eje_us += v->npt;
    }
}

/* Revierte los contadores de un vehiculo (para bajar del ferry) */
void desembarcar_cnt(Ferry *f, const Veh *v) {
    int lp = lleva_pas(v);
    f->ton_car -= v_ton(v);
    f->pas--;
    if (v->tpa==0) f->vip_us--; else f->eje_us--;
    if (lp) {
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
    int i;
    if (f->n_veh >= f->cap_veh) return 1;   /* lleno en vehiculos          */
    if (f->pas   >= f->cap_pas) return 1;   /* lleno en pasajeros          */
    if (f->n_veh < f->min_veh)  return 0;   /* no alcanza minimo           */
    if (q_vacia(cola))          return 1;   /* minimo OK + cola vacia      */
    /* Minimo OK: zarpa si ningun vehiculo de la cola puede abordar */
    for (i = 0; i < cola->size; i++) {
        int pos = (cola->head + i) % MAX_VEH;
        if (puede_subir(f, &lista[cola->buf[pos]]))
            return 0;   /* al menos uno puede subir, no zarpa aun */
    }
    return 1;
}

/* ── Reporte ──────────────────────────────────────────────────────────────── */
void rep_viaje(const Ferry *f, Veh lista[], int num_carga, FILE *out) {
    int   i;
    float ing = 0.0f;
    for (i = 0; i < f->n_veh; i++) {
        const Veh *v = &lista[f->abordo[i]];
        ing += ing_veh(v, f->tipo) + ing_pas(v, f->tipo);
    }
    /* Imprime en pantalla y archivo */
    FILE *dest[2] = { stdout, out };
    int   d;
    for (d = 0; d < 2; d++) {
        if (!dest[d]) continue;
        fprintf(dest[d], "Carga Nro. %d:\n", num_carga);
        fprintf(dest[d], "Ferry %s\n", f->nombre);
        fprintf(dest[d], "Viaje Nro. %d\n", f->n_viajes);
        fprintf(dest[d], "Num. vehiculos: %d\n", f->n_veh);
        fprintf(dest[d], "Num. pasajeros: %d (%d mayores de 60 anos)\n",
                f->pas, f->pas_3ra);
        fprintf(dest[d], "Peso: %.2f toneladas\n", f->ton_car);
        fprintf(dest[d], "Ingreso: %.0f BsF.\n", ing);
        for (i = 0; i < f->n_veh; i++) {
            const Veh *v = &lista[f->abordo[i]];
            fprintf(dest[d], "ID: %-12s Tipo: %s\n", v->placa, nom_tipo(tipo_v(v)));
        }
        fprintf(dest[d], "\n");
    }
}

void rep_stats(const Stats *s, FILE *out) {
    int mejor = 1, i;
    FILE *dest[2] = { stdout, out };
    int   d;
    for (i = 2; i <= 7; i++)
        if (s->frec[i] > s->frec[mejor]) mejor = i;
    for (d = 0; d < 2; d++) {
        if (!dest[d]) continue;
        fprintf(dest[d], "Estadisticas:\n");
        fprintf(dest[d], "Total vehiculos transportados: %d\n",    s->tot_veh);
        fprintf(dest[d], "Total pasajeros transportados: %d\n",    s->tot_pas);
        fprintf(dest[d], "Total de ingresos: %.0f BsF.\n",         s->tot_ing);
        fprintf(dest[d], "Num. pasajeros no trasladados: %d\n",    s->pas_perd);
        fprintf(dest[d], "Vehiculo mas frecuente: %s\n",           nom_tipo(mejor));
        fprintf(dest[d], "Mayor frec. vehiculos en espera: %d (a las %04d)\n",
                s->max_cola, s->h_cola);
    }
}

/* ── Simulacion ───────────────────────────────────────────────────────────── */
void simular(Veh lista[], int n, int orden[3], Ferry flota[], FILE *out) {
    Cola q_exp, q_trad, q_emerg, q_muelle;
    Stats st;
    int prox_veh, min_act, min_fin, en_muelle, t_libre, n_carga, fi, k;

    q_ini(&q_exp); q_ini(&q_trad); q_ini(&q_emerg); q_ini(&q_muelle);

    st.tot_veh=0; st.tot_pas=0; st.pas_perd=0;
    st.tot_ing=0.0f; st.max_cola=0; st.h_cola=0;
    for (k=0; k<8; k++) st.frec[k]=0;

    /* Cola de muelle segun orden del archivo (IDs 1,2,3 → indices 0,1,2) */
    for (k = 0; k < N_FERRYS; k++) q_push(&q_muelle, orden[k] - 1);

    min_act = h_a_min(lista[0].hora);
    min_fin = min_act + 1440;
    prox_veh = 0;
    n_carga  = 0;
    en_muelle = q_pop(&q_muelle);
    flota[en_muelle].estado = CARGANDO;
    t_libre = min_act;

    while (min_act <= min_fin) {

        /* 1. Encolar vehiculos que llegan en este minuto */
        while (prox_veh < n && h_a_min(lista[prox_veh].hora) == min_act) {
            int idx = prox_veh++;
            if (es_emerg(&lista[idx]))          q_push(&q_emerg, idx);
            else if (lista[idx].tipo_ferry == 0) q_push(&q_exp,   idx);
            else                                 q_push(&q_trad,  idx);
        }

        /* 2. Estadisticas de espera (se mide tras llegadas, antes de cargar) */
        {
            int en_espera = q_exp.size + q_trad.size + q_emerg.size;
            if (en_espera > st.max_cola) {
                st.max_cola = en_espera;
                st.h_cola   = min_a_h(min_act);
            }
        }

        /* 2. Carga del ferry en el muelle */
        if (en_muelle >= 0 && min_act >= t_libre) {
            Ferry *f = &flota[en_muelle];
            Cola  *qf = (f->tipo == EXPRESO) ? &q_exp : &q_trad;

            /* Prioridad 1: emergencias */
            if (!q_vacia(&q_emerg)) {
                int eidx = q_pop(&q_emerg);
                /* Si ferry lleno, baja el ultimo vehiculo normal */
                while (f->n_veh >= f->cap_veh) {
                    int last = f->abordo[--f->n_veh];
                    desembarcar_cnt(f, &lista[last]);
                    Cola *dest = (lista[last].tipo_ferry==0) ? &q_exp : &q_trad;
                    q_push_f(dest, last);
                }
                embarcar(f, lista, eidx);
                t_libre = min_act + T_CARGA;

            /* Prioridad 2: cola normal del ferry */
            } else if (!q_vacia(qf)) {
                int frente = q_ver(qf);
                if (puede_subir(f, &lista[frente])) {
                    q_pop(qf);
                    embarcar(f, lista, frente);
                    t_libre = min_act + T_CARGA;
                }
            }

            /* Verifica zarpe (no zarpa si hay emergencias esperando) */
            if (q_vacia(&q_emerg) && puede_zarpar(f, qf, lista)) {
                /* Acumula estadisticas del viaje */
                for (fi = 0; fi < f->n_veh; fi++) {
                    Veh *v = &lista[f->abordo[fi]];
                    st.tot_veh++;
                    st.frec[tipo_v(v)]++;
                    st.tot_ing += ing_veh(v, f->tipo) + ing_pas(v, f->tipo);
                    st.tot_pas += 1 + (lleva_pas(v) ? v->npa + v->npt : 0);
                }
                f->n_viajes++;
                n_carga++;
                rep_viaje(f, lista, n_carga, out);

                /* Reinicia ferry y lo pone en viaje */
                f->estado = EN_VIAJE;
                f->t_rest = f->t_ciclo;
                f->n_veh=0; f->pas=0; f->pas_3ra=0;
                f->vip_us=0; f->eje_us=0; f->ton_car=0.0f;

                /* Siguiente ferry al muelle */
                en_muelle = q_vacia(&q_muelle) ? -1 : q_pop(&q_muelle);
                if (en_muelle >= 0) {
                    flota[en_muelle].estado = CARGANDO;
                    t_libre = min_act;
                }
            }
        } else if (en_muelle < 0 && !q_vacia(&q_muelle)) {
            en_muelle = q_pop(&q_muelle);
            flota[en_muelle].estado = CARGANDO;
            t_libre = min_act;
        }

        /* 4. Avanza ferrys en viaje */
        for (fi = 0; fi < N_FERRYS; fi++) {
            if (flota[fi].estado == EN_VIAJE && --flota[fi].t_rest <= 0) {
                flota[fi].estado = EN_ESPERA;
                q_push(&q_muelle, fi);
            }
        }

        /* 5. Condicion de fin */
        if (prox_veh >= n &&
            q_exp.size==0 && q_trad.size==0 && q_emerg.size==0) {
            int alguno_listo = (en_muelle >= 0 &&
                                flota[en_muelle].n_veh >= flota[en_muelle].min_veh);
            if (!alguno_listo) break;
        }

        min_act++;
    }

    /* Pasajeros no trasladados (en colas de espera sobrantes) */
    {
        Cola *sobrantes[3] = { &q_emerg, &q_exp, &q_trad };
        int  s;
        for (s = 0; s < 3; s++) {
            for (k = 0; k < sobrantes[s]->size; k++) {
                int pos = (sobrantes[s]->head + k) % MAX_VEH;
                Veh *v  = &lista[sobrantes[s]->buf[pos]];
                st.pas_perd += 1 + (lleva_pas(v) ? v->npa + v->npt : 0);
            }
        }
    }

    /* Pasajeros en el ferry del muelle que no alcanzo el minimo para zarpar */
    if (en_muelle >= 0 && flota[en_muelle].n_veh > 0) {
        Ferry *f = &flota[en_muelle];
        for (k = 0; k < f->n_veh; k++) {
            const Veh *v = &lista[f->abordo[k]];
            st.pas_perd += 1 + (lleva_pas(v) ? v->npa + v->npt : 0);
        }
    }

    rep_stats(&st, out);
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