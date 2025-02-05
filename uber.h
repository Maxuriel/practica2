/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _UBER_H_RPCGEN
#define _UBER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct InfoAuto {
	int disponible;
	float x;
	float y;
	char *tipoUber;
	float tarifa;
	char *placas;
};
typedef struct InfoAuto InfoAuto;

struct Solicitud {
	float x_pasajero;
	float y_pasajero;
};
typedef struct Solicitud Solicitud;

struct TerminarViajeArgs {
	char *placas;
	float costo;
	char *posicion_final;
};
typedef struct TerminarViajeArgs TerminarViajeArgs;

struct Estado {
	int viajes_realizados;
	int autos_disponibles;
	float ganancia_total;
};
typedef struct Estado Estado;

#define UBERPROG 0x20000001
#define UBERVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define SolicitarViaje 1
extern  InfoAuto * solicitarviaje_1(Solicitud *, CLIENT *);
extern  InfoAuto * solicitarviaje_1_svc(Solicitud *, struct svc_req *);
#define TerminarViaje 2
extern  void * terminarviaje_1(TerminarViajeArgs *, CLIENT *);
extern  void * terminarviaje_1_svc(TerminarViajeArgs *, struct svc_req *);
#define EstadoServicio 3
extern  Estado * estadoservicio_1(void *, CLIENT *);
extern  Estado * estadoservicio_1_svc(void *, struct svc_req *);
extern int uberprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SolicitarViaje 1
extern  InfoAuto * solicitarviaje_1();
extern  InfoAuto * solicitarviaje_1_svc();
#define TerminarViaje 2
extern  void * terminarviaje_1();
extern  void * terminarviaje_1_svc();
#define EstadoServicio 3
extern  Estado * estadoservicio_1();
extern  Estado * estadoservicio_1_svc();
extern int uberprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_InfoAuto (XDR *, InfoAuto*);
extern  bool_t xdr_Solicitud (XDR *, Solicitud*);
extern  bool_t xdr_TerminarViajeArgs (XDR *, TerminarViajeArgs*);
extern  bool_t xdr_Estado (XDR *, Estado*);

#else /* K&R C */
extern bool_t xdr_InfoAuto ();
extern bool_t xdr_Solicitud ();
extern bool_t xdr_TerminarViajeArgs ();
extern bool_t xdr_Estado ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_UBER_H_RPCGEN */
