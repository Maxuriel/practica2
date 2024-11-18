#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "uber.h"

#define NUM_AUTOS 8

typedef struct {
    int disponible;
    float x, y;
    char tipoUber[20];
    float tarifa;
    char placas[10];
} Auto;

static Auto autos[NUM_AUTOS];
static int viajes_realizados = 0;
static float ganancia_total = 0;

// Inicializar autos
void inicializar_autos() {
    srand(time(NULL));
    printf("[DEBUG] Inicializando autos...\n");
    for (int i = 0; i < NUM_AUTOS; i++) {
        autos[i].disponible = 1;
        autos[i].x = rand() % 50;
        autos[i].y = rand() % 50;

        // Asigna nombres específicos a los tipos de Uber
        if (i % 3 == 0) {
            strncpy(autos[i].tipoUber, "UberPlanet", sizeof(autos[i].tipoUber) - 1);
        } else if (i % 3 == 1) {
            strncpy(autos[i].tipoUber, "UberXL", sizeof(autos[i].tipoUber) - 1);
        } else {
            strncpy(autos[i].tipoUber, "UberBlack", sizeof(autos[i].tipoUber) - 1);
        }
        autos[i].tipoUber[sizeof(autos[i].tipoUber) - 1] = '\0'; // Asegura terminación nula

        autos[i].tarifa = (i % 3 == 0) ? 10 : (i % 3 == 1) ? 15 : 25;
        snprintf(autos[i].placas, sizeof(autos[i].placas), "ABC%d%d%d", i, i, i);

        printf("Auto %d inicializado: disponible=%d, posición=(%.2f, %.2f), tipo=%s, tarifa=%.2f, placas=%s\n",
               i, autos[i].disponible, autos[i].x, autos[i].y, autos[i].tipoUber, autos[i].tarifa, autos[i].placas);
    }
    printf("[DEBUG] Autos inicializados correctamente.\n");
}

// Función para inicializar el servidor
void init_server() {
    inicializar_autos();
}

// Solicitar un viaje
InfoAuto* solicitarviaje_1_svc(Solicitud *solicitud, struct svc_req *req) {
    static InfoAuto respuesta;  // Respuesta estática para evitar problemas de memoria
    int index = -1;             // Índice del auto seleccionado
    float min_distancia = 1e9;  // Inicializa distancia mínima como un valor grande

    // Libera memoria previamente asignada a respuesta.tipoUber y respuesta.placas
    if (respuesta.tipoUber) {
        free(respuesta.tipoUber);
        respuesta.tipoUber = NULL;
    }
    if (respuesta.placas) {
        free(respuesta.placas);
        respuesta.placas = NULL;
    }

    printf("[DEBUG] Solicitud de viaje recibida: posición pasajero=(%.2f, %.2f)\n",
           solicitud->x_pasajero, solicitud->y_pasajero);

    // Busca el auto más cercano
    for (int i = 0; i < NUM_AUTOS; i++) {
        if (autos[i].disponible) {
            float distancia = sqrt(pow(autos[i].x - solicitud->x_pasajero, 2) +
                                   pow(autos[i].y - solicitud->y_pasajero, 2));
            printf("[DEBUG] Auto %d: posición=(%.2f, %.2f), distancia=%.2f, disponible=%d\n",
                   i, autos[i].x, autos[i].y, distancia, autos[i].disponible);

            if (distancia < min_distancia) {
                min_distancia = distancia;
                index = i;
                printf("[DEBUG] Auto %d seleccionado como más cercano.\n", i);
            }
        }
    }

    // Si no se encuentra un auto disponible
    if (index == -1) {
        printf("[DEBUG] No se encontraron autos disponibles.\n");
        return NULL;
    }

    // Actualiza el auto seleccionado como no disponible
    autos[index].disponible = 0;

    // Asigna memoria para las cadenas en respuesta
    respuesta.tipoUber = strdup(autos[index].tipoUber);
    respuesta.placas = strdup(autos[index].placas);

    // Copia los datos del auto seleccionado a respuesta
    respuesta.disponible = 1;
    respuesta.x = autos[index].x;
    respuesta.y = autos[index].y;
    respuesta.tarifa = autos[index].tarifa;

    printf("[DEBUG] Auto asignado: %s, tipo=%s, posición=(%.2f, %.2f), tarifa=%.2f\n",
           respuesta.placas, respuesta.tipoUber, respuesta.x, respuesta.y, respuesta.tarifa);

    return &respuesta;
}

// Terminar un viaje
void* terminarviaje_1_svc(TerminarViajeArgs *args, struct svc_req *req) {
    printf("[DEBUG] Terminar viaje recibido: placas=%s, costo=%.2f, posición final=%s\n",
           args->placas, args->costo, args->posicion_final);

    for (int i = 0; i < NUM_AUTOS; i++) {
        if (strcmp(autos[i].placas, args->placas) == 0) {
            autos[i].disponible = 1;
            sscanf(args->posicion_final, "%f,%f", &autos[i].x, &autos[i].y);
            ganancia_total += args->costo;
            viajes_realizados++;
            printf("[DEBUG] Viaje terminado: auto=%s, nueva posición=(%.2f, %.2f), costo=%.2f\n",
                   autos[i].placas, autos[i].x, autos[i].y, args->costo);
            break;
        }
    }
    return NULL;
}

// Consultar estado del servicio
Estado* estadoservicio_1_svc(void *argp, struct svc_req *req) {
    static Estado estado;

    estado.viajes_realizados = viajes_realizados;
    estado.autos_disponibles = 0;

    for (int i = 0; i < NUM_AUTOS; i++) {
        if (autos[i].disponible) {
            estado.autos_disponibles++;
        }
    }
    estado.ganancia_total = ganancia_total;

    printf("[DEBUG] Estado del servicio: viajes_realizados=%d, autos_disponibles=%d, ganancia_total=%.2f\n",
           estado.viajes_realizados, estado.autos_disponibles, estado.ganancia_total);

    return &estado;
}

