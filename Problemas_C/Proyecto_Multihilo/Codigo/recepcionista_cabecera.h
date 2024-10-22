#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>

#include "semaforoConCola.h"

#ifndef recepcionista_h
#define recepcionista_h

#ifndef nRecepcionista
#define nRecepcionista 15
#endif


/*Mutex recepcionista-visitante*/
extern pthread_mutex_t atencionCliente[nRecepcionista];
extern pthread_mutex_t mVisit[nRecepcionista];
/*Mutex recepcionista-audio guia*/
extern pthread_mutex_t senalGuia;    
extern pthread_mutex_t recibidoGuia; 
/*Mutex recepcionista-guia*/
extern pthread_mutex_t senalAudioGuiaRecep;
extern pthread_mutex_t senalAudioGuia;

/*Condiciones del mutex recepcionista-visitante*/
extern pthread_cond_t condAtencionCliente[nRecepcionista];
extern pthread_cond_t condMVisit[nRecepcionista];
/*Condiciones del mutex recepcionista-audio guia*/
extern pthread_cond_t condSenalAudioGuiaRecep;
extern pthread_cond_t condSenalAudioGuia;
/*Condiciones del mutex recepcionista-guia*/
extern pthread_cond_t condSenalGuia;
extern pthread_cond_t condSenalGuiaRecep;

/*Banderas del mutex recepcionista-visitante*/
extern int banderaVisit[nRecepcionista];
extern int banderaRecep[nRecepcionista];
/*Banderas del mutex recepcionista-audio guia*/
extern int banderaSenalRecepcionista;
extern int banderaSenalAudioGuia;
/*Banderas del mutex recepcionista-guia*/
extern int banderaSenalGuiaRecepcionista;
extern int banderaSenalGuia;


extern bool guiaActivado;                    //Indica si el hilo guia esta iniciado
extern bool ban[nRecepcionista];             // Variable que indica si el recepcionista inicio
extern bool tipoVisita[nRecepcionista];      //Variable que indica el tipo de visita, con audioguia o con guia
extern bool visitaTerminada[nRecepcionista]; //Señala la entrada o salida del museo del cliente
extern bool audioGuiaDevuelto;               //Indica si el cliente pide o devuelve un AudioGuia
extern bool salaEspera[nRecepcionista];      //Guarda la entrada o salida del cliente siendo atendido
extern bool audioGuiaActivado;               //Indica si el hilo AudioGuia esta activado
extern bool prioridad;                       //Indica si el visitante es nuevo o regular
extern bool prioridadGuia[nRecepcionista];   //Guarda la prioridad el visitante en un buffer
extern bool recorridoTerminado;              //Indica si el visitante entra o sale del museo

extern int ID_hilo[nRecepcionista];          //Guarda el hilo de los clientes que estan siendo atendidos
extern int ID_HILO;                          //Indice de ID_hilo
extern int ID_salaEspera;                    //Indica el indice en salaEspera(cola en espera por AudioGuia)
extern int ID_HILO_GUIA;                     //Guarda el indice el hilo
extern int ID_salaEsperaGuia;                //Indice para el vector salaEspera
extern int guiandoV[nRecepcionista];         //Guarda el guia del recorrido en un buffer
extern int tiempoUsoV[nRecepcionista];       //Guarda el tiempo de recorrido en un buffer
extern int tiempoRecorrido;                  //Tiempo de recorrido del guia
extern int guiaSaliendo;                     //Indica el proximo guia en salir
extern int audioGuiaUsado;                   //ID del audio guia que se esta devolviendo
extern int tiempoUsoAudioGuia;               //Cantidad de tiempo que se uso un audio guia
extern int idRecepcionista;                  //Guarda el ID del recepcionista que busca un audio guia
extern int ingresoRecepcion[nRecepcionista]; //Guarda los pagos de la recepcion
extern int numVisitantesHora[nRecepcionista];//Guarda el numero de visitantes por hora

extern semCola mensGuia;                     //Pide permiso para un guia
extern semCola mensAudioGuia;                //Pide permiso para un AudioGuia
extern semCola sRecept[nRecepcionista];      //Semaforo con cola con todos los visitantes por atender el recepcionista #i

extern bool diaTerminado;

int devolviendoAudioGuia[nRecepcionista];    //Guarda la ID del audio guia que el recepcionista #i devuelve
int tiempoAudioGuia[nRecepcionista];         //Tiempo de uso del audio guia devuelto a recepcion


void* recept(void* ID){
	int idHilo = ((id_h*)ID)->id;
	
	//Imprime en pantalla el ID del recepcionista
	printf(" Hilo Recepcionista %i ACTIVADO\n",  idHilo + 1); 
	
	while(!audioGuiaActivado || !guiaActivado);     //Espera que el hilo AudioGuia se active
	pthread_mutex_lock(&mVisit[idHilo]);
	ban[idHilo] = true;                             //Activa la bandera del recepcionista para que cliente continue
	
	/*El recepcionista se queda en bucle esperando a un visitante*/
	while(1){	
		/*Espera señal de visitante*/	
		while(!banderaRecep[idHilo]){
			pthread_cond_wait(&condMVisit[idHilo], &mVisit[idHilo]);
		}
		banderaRecep[idHilo] = 0;
		
		//Atendiendo al cliente
		printf(" \nRecepcionista: %i atendiendo a cliente: %i\n", idHilo + 1, ID_hilo[idHilo]);
		
	
		if(!diaTerminado || visitaTerminada[idHilo]){
			if(!visitaTerminada[idHilo]){
				if(tipoVisita[idHilo]){
					ingresoRecepcion[idHilo] += 20;
				}else{
					ingresoRecepcion[idHilo] += 10;
				}				
			}
			numVisitantesHora[idHilo]++;
			
			if(tipoVisita[idHilo]){
				/*El visitante pide un guia*/
				semCola_wait(&mensGuia);                          //Recepcionista pide permiso para buscar un AudioGuia
					/*Se guarda los datos del visitante en las variables de guia*/				
					ID_HILO_GUIA = ID_hilo[idHilo];               //Se guarda el ID del cliente
					ID_salaEsperaGuia = idHilo;                   //Se guarda el estado de entrada (salindo)
					prioridad = prioridadGuia[idHilo];            //Se guarda la prioridad del visitante
					recorridoTerminado = visitaTerminada[idHilo]; //Indica si el cliente espara por un guia
			
					pthread_mutex_lock(&senalGuia);
						pthread_mutex_lock(&recibidoGuia);
							banderaSenalGuia = 1;
							pthread_cond_signal(&condSenalGuia);     //Envia señal a audio guia
							guiandoV[idHilo] = guiaSaliendo;
					pthread_mutex_unlock(&senalGuia);
							while(!banderaSenalGuiaRecepcionista){
	    						pthread_cond_wait(&condSenalGuiaRecep, &recibidoGuia); //Espera por señal de audio guia
							}
							banderaSenalGuiaRecepcionista = 0;
						pthread_mutex_unlock(&recibidoGuia);
							
					/*Se guardan los resultados de guia en el buffer del recepcionista*/
					tiempoUsoV[idHilo] = tiempoRecorrido;
				semCola_signal(&mensGuia);                        //Regresa el permiso para que otro lo use
			}else{
				/*El visitante pide un audio guia*/
				semCola_wait(&mensAudioGuia);                     //Recepcionista pide permiso para buscar un AudioGuia
					/*Se guardan los datos del visitante en las variables de audio guia*/				
					idRecepcionista = idHilo;                     //ID del recepcionista que devuelve un audio guia
					audioGuiaUsado = devolviendoAudioGuia[idHilo];//ID del audio guia que devuelve el recepcionista
					tiempoUsoAudioGuia = tiempoAudioGuia[idHilo]; //Tiempo de uso del audio guia devuelto por el recepcionista
					ID_HILO = ID_hilo[idHilo];                    //Se guarda el ID del cliente
					ID_salaEspera = idHilo;                       //Se guarda el estado de entrada (salindo)
					audioGuiaDevuelto = visitaTerminada[idHilo];  //Indica si el cliente devuelve o pide un AudioGuia (devolviendo)
						
					pthread_mutex_lock(&senalAudioGuia);
						pthread_mutex_lock(&senalAudioGuiaRecep);
							banderaSenalAudioGuia = 1;
							pthread_cond_signal(&condSenalAudioGuia);     //Envia una señal a audio guia
					pthread_mutex_unlock(&senalAudioGuia);
							while(!banderaSenalRecepcionista){
	    						pthread_cond_wait(&condSenalAudioGuiaRecep, &senalAudioGuiaRecep); //Espera señal de audio guia
							}
							banderaSenalRecepcionista = 0;
						pthread_mutex_unlock(&senalAudioGuiaRecep);	
				semCola_signal(&mensAudioGuia);                    //Regresa el permiso para que otro lo use
			}
		}
		/*Envia señal a visitante*/
		pthread_mutex_lock(&atencionCliente[idHilo]);
			banderaVisit[idHilo] = 1;
			pthread_cond_signal(&condAtencionCliente[idHilo]);	
		pthread_mutex_unlock(&atencionCliente[idHilo]);		
	}
	return NULL;
}

#endif