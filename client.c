#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "uber.h"

void pasajero(CLIENT* clnt) {
    Solicitud solicitud;
    printf("Ingrese la posición de origen (x, y): ");
    scanf("%f %f", &solicitud.x_pasajero, &solicitud.y_pasajero);

    InfoAuto* auto_asignado = solicitarviaje_1(&solicitud, clnt);
    if (auto_asignado == NULL) {
        printf("No se encontró un auto disponible.\n");
        return;
    }

    printf("Auto asignado:\n");
    printf("  Placas: %s\n", auto_asignado->placas);
    printf("  Tipo: %s\n", auto_asignado->tipoUber);
    printf("  Tarifa: %.2f/km\n", auto_asignado->tarifa);

    float x_dest, y_dest;
    printf("Ingrese la posición de destino (x, y): ");
    scanf("%f %f", &x_dest, &y_dest);

    float distancia = sqrt(pow(x_dest - auto_asignado->x, 2) + pow(y_dest - auto_asignado->y, 2));
    printf("Distancia del viaje: %.2f km\n", distancia);
    printf("Simulando viaje...\n");
    sleep((int)(distancia * 2));  // Simulación basada en la distancia

    TerminarViajeArgs args;
    args.placas = auto_asignado->placas;
    args.costo = distancia * auto_asignado->tarifa;
    char posicion_final[20];
    snprintf(posicion_final, sizeof(posicion_final), "%.2f,%.2f", x_dest, y_dest);
    args.posicion_final = posicion_final;

    terminarviaje_1(&args, clnt);
    printf("Viaje terminado. Costo: %.2f\n", args.costo);
}

void administrador(CLIENT* clnt) {
    while (1) {
        Estado* estado = estadoservicio_1(NULL, clnt);
        if (estado != NULL) {
            printf("Estado del servicio:\n");
            printf("  Viajes realizados: %d\n", estado->viajes_realizados);
            printf("  Autos disponibles: %d\n", estado->autos_disponibles);
            printf("  Ganancia total: %.2f\n", estado->ganancia_total);
        }
        sleep(2);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <servidor> <rol (pasajero/administrador)>\n", argv[0]);
        return 1;
    }

    CLIENT* clnt = clnt_create(argv[1], UBERPROG, UBERVERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    if (strcmp(argv[2], "pasajero") == 0) {
        pasajero(clnt);
    } else if (strcmp(argv[2], "administrador") == 0) {
        administrador(clnt);
    } else {
        printf("Rol desconocido: %s\n", argv[2]);
    }

    clnt_destroy(clnt);
    return 0;
}
