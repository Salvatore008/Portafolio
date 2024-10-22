#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#define nRecepcionista 15     //Numero de recepcionistas
#define nGuias 10
#define nGrupo 10             //Numero maximo y minimo del grupo a cargo de un guia
#define nAudioGuias 30         //Numero de audioguias


#include "semaforoConCola.h"
#include "recepcionista_cabecera.h"
#include "visitante.h"
#include "audioGuia.h"
#include "guia.h"
#include "time.h"


/*Mutex de sincronizacion entre recepcionista y visitante*/
pthread_mutex_t mVisit[nRecepcionista];
pthread_mutex_t atencionCliente[nRecepcionista];
/*Mutex de sincronizacion entre recepcionista y audio guia*/
pthread_mutex_t senalAudioGuia;
pthread_mutex_t senalAudioGuiaRecep;
pthread_mutex_t mutexAudioGuiaDesencolar;
/*Mutex de sincronizacion entre recepcionista y guia*/
pthread_mutex_t senalGuia;    
pthread_mutex_t recibidoGuia; 
pthread_mutex_t mutexGuiaDesencolar;

/*Condiciones para el bloqueo de mutex entre recepcionista y visitante*/
pthread_cond_t condMVisit[nRecepcionista];
pthread_cond_t condAtencionCliente[nRecepcionista];
/*Condiciones para el bloqueo de mutex entre recepcionista y audio guia*/
pthread_cond_t condSenalAudioGuia;
pthread_cond_t condSenalAudioGuiaRecep;
pthread_cond_t condAudioGuiaDesencolar;
/*Condiciones para el bloqueo de mutex entre recepcionista y guia*/
pthread_cond_t condSenalGuia;
pthread_cond_t condSenalGuiaRecep;
pthread_cond_t condGuiaDesencolar;

/*Bandera para el correcto funcionamiento del mutex recepcionista-visitante*/
int banderaVisit[nRecepcionista];
int banderaRecep[nRecepcionista];
/*Bandera para el correcto funcionamiento del mutex recepcionista-audio guia*/
int banderaSenalRecepcionista;
int banderaSenalAudioGuia;
int banderaDesencolar;
/*Bandera para el correcto funcionamiento del mutex recepcionista-guia*/
int banderaSenalGuiaRecepcionista;
int banderaSenalGuia;
int banderaGuiaDesencolar;

/*Colas de espera*/
semCola sRecept[nRecepcionista];        //Visitantes esperando ser atendido por el recepcionista #i
semCola mensAudioGuia;                  //Recepcionista revisando si hay un audio guia disponible sino ingresa el visitante a la sala de espera
semCola mensGuia;                       //Recepcionista asigna un guia o ingresa a la sala de espera al visitantes

/*Cola de espera por reenvolso*/
sem_t reenvolsoGuia;                    //sirve para sincronizar el reenvolso por no conseguir un guia

/*Banderas que indician el inicio de un hilo*/
bool ban[nRecepcionista];               //Indica si recepcionista #i se inicio
bool audioGuiaActivado;         //Indica si el hilo audio guia se inicio
bool guiaActivado;              //Indica si el hilo guia se inicio

/*Variables recepcionista-visitante*/
bool tipoVisita[nRecepcionista];        //Variable que indica el tipo de visita, con audioguia o con guia
bool visitaTerminada[nRecepcionista];   //Señala la entrada o salida del museo del cliente
bool prioridadGuia[nRecepcionista];
int guiandoV[nRecepcionista];
int tiempoUsoV[nRecepcionista];

/*Variables visitante-audio guia*/
bool audioGuiaLibre;            //Sale un cliente de la sala de espera
bool salaEspera[nRecepcionista];        //Guarda la entrada o salida del cliente siendo atendido

/*Variables recepcionista-visitante-guia*/
bool salaEsperaGuia[nRecepcionista];

/*Variables visitante-guia*/
bool guiaLibre;

/*Variable guia*/
int nGuiasDisponibles;

/*Variables hilos*/
int ID_hilo[nRecepcionista];            //Guarda el hilo de los clientes que estan siendo atendidos
pthread_t recepts[nRecepcionista];	    //Numero de hilos recepcionista

/*Dia*/
bool diaTerminado;              //Indica si el dia a terminado
long long hora;                 //Duracion del dia

/*Variables para contabilizar el ingreso del museo*/
int numVisitantesHora[nRecepcionista];
int numGuiasActivosHora;
int ingresoRecepcion[nRecepcionista];
int ingresoTotal;
extern int nPropinas[nGuias];
extern int duracionRecorridos[nGuias];
extern int tiempoTotalAudioGuia[nAudioGuias];