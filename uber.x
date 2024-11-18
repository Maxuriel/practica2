struct InfoAuto {
    int disponible;         /* 1 si el auto está disponible */
    float x;                /* Coordenada X */
    float y;                /* Coordenada Y */
    string tipoUber<20>;    /* Tipo de Uber */
    float tarifa;           /* Tarifa por km */
    string placas<10>;      /* Placas */
};

struct Solicitud {
    float x_pasajero;       /* Coordenada X del pasajero */
    float y_pasajero;       /* Coordenada Y del pasajero */
};

struct TerminarViajeArgs {
    string placas<10>;      /* Placas del auto */
    float costo;            /* Costo del viaje */
    string posicion_final<20>; /* Posición final en formato "x,y" */
};

struct Estado {
    int viajes_realizados;  /* Número de viajes realizados */
    int autos_disponibles;  /* Número de autos disponibles */
    float ganancia_total;   /* Ganancias acumuladas */
};

program UBERPROG {
    version UBERVERS {
        InfoAuto SolicitarViaje(Solicitud) = 1;
        void TerminarViaje(TerminarViajeArgs) = 2;
        Estado EstadoServicio(void) = 3;
    } = 1;
} = 0x20000001;
