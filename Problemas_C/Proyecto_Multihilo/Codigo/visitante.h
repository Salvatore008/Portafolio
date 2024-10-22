#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "semaforoConCola.h"
#include "cola.h"

#ifndef visitante_h
#define visitante_h

#ifndef nRecepcionista
#define nRecepcionista 15
#endif

/*Mutex de recepcionista-visitante*/
extern pthread_mutex_t mVisit[nRecepcionista];
extern pthread_mutex_t atencionCliente[nRecepcionista];
/*Mutex de visitante-audio guia*/
extern pthread_mutex_t  mutexAudioGuiaDesencolar;
/*Mutex de recepcionista(visitante usa el mutex)-guia*/
extern pthread_mutex_t senalGuia;    
extern pthread_mutex_t recibidoGuia; 
extern pthread_mutex_t mutexGuiaDesencolar;

/*Condiciones de mutex recepcionista-visitante*/
extern pthread_cond_t condMVisit[nRecepcionista];
extern pthread_cond_t condAtencionCliente[nRecepcionista];
/*Condiciones de mutex visitante-audio guia*/
extern pthread_cond_t condAudioGuiaDesencolar;
/*Condiciones de mutex visitante-guia*/
extern pthread_cond_t condSenalGuia;
extern pthread_cond_t condSenalGuiaRecep;
extern pthread_cond_t condGuiaDesencolar;

/*Bandera de mutex recepcionista-visitante*/
extern int banderaVisit[nRecepcionista];
extern int banderaRecep[nRecepcionista];
/*Bandera de mutex visitante-audio guia*/
extern int banderaDesencolar;
/*Bandera de mutex recepcionista(visitante usa el mutex)-guia*/
extern int banderaSenalGuiaRecepcionistaGuia;
extern int banderaSenalGuia;
extern int banderaGuiaDesencolar;
extern int banderaSenalGuiaRecepcionista;

/*Variables visitante-recepcionista-audioGuia-time*/
extern bool diaTerminado;

/*Variables recepcionoista-visitante-guia*/
extern bool recorridoTerminado;               //Indica si termino el recorrido
extern bool salaEsperaGuia[nRecepcionista];   //Indica si el visitante entra a la sala de espera
extern int guiaSaliendo;					  //Indica el proximo guia en salir
extern int tiempoRecorrido;					  //Tiempo del recorrido
extern semCola mensGuia;                      //Cola de espera para ser atendido por un guia (en esta caso es para la propina)

/*Variables visitante-recepcionista-audio guia*/
extern int audioGuiaUsado;                    //ID del audio guia que se esta devolviendo

/*Variables recepcionista-visitante*/
extern bool visitaTerminada[nRecepcionista];  //Indica si el visitante entra o se va del museo
extern bool salaEspera[nRecepcionista];       //Indica si el visitante entra o no a la sal de espera (para entrar a la cola por Audioguia)
extern bool ban[nRecepcionista];              //Siver para señalar que el hilo recepcionista #o ya inicio
extern bool tipoVisita[nRecepcionista];       //Variable que indica el tipo de visita con audioguia/guia
extern bool prioridadGuia[nRecepcionista];    //Guarda la prioridad del visitante
extern int ID_hilo[nRecepcionista];           //Indica el numero de recepcionista que atendera al cliente
extern int guiandoV[nRecepcionista];          //Guarda el guia del visitatne en un buffer
extern int tiempoUsoV[nRecepcionista];        //Guarda el tiempo de recorrido en un buffer

/*Variable recepcionista-visitante*/
extern semCola sRecept[nRecepcionista];          //Semaforo con cola con todos los visitantes por atender el recepcionista #i
extern int devolviendoAudioGuia[nRecepcionista]; //Indica el ID del audio guia que esta devolviendo el recepcionista #i
extern int tiempoAudioGuia[nRecepcionista];      //Tiempo que a sido usado el audio guia #i

/*Variables visitante-audio guia*/
extern bool audioGuiaLibre;                   //Indica que hay un AudioGuia disponible (sale de la cola de espera)
extern Cola colAudioGuia;                     //Cola de espera por AudioGuia
extern int idAudioGuia[nRecepcionista];       //ID del audio guia que el recepcionista #i le dio a un visitante

/*Variables visitante-guia*/
extern bool guiaLibre;                    //Indica que hay un guia disponible para salir
extern int guiaEntrando;                  //ID del guia que termino el recorrido
extern int ID_HILO_GUIA;                  //ID del visitante esperando por un guia
extern int propina;                       //Propina del visitante #i
extern Cola colGuia;                      //Cola de espera regular por guia
extern Cola colGuiaPrioridad;             //Cola de espera VIP por guia

/*Variables que contabilizan el ingreso total del museo*/
extern int ingresoTotal;
extern sem_t reenvolsoGuia;
extern long long hora;

void* visit(void* ID){
	int idHilo = ((id_visit*)ID)->idRecept->id;              //Indica el ID del recepcionista que atendera al cliente
	int idVisitante = *(((id_visit*)ID)->idVisit);
	/*Se crea una semilla para obtener un nunmero aleatorio*/
	unsigned int seed = time(NULL) + pthread_self();
    srand(seed);
    
	int numRamdon = rand() % 2;
	bool tipoGuia = numRamdon == 1;           //Indica el tipo de visita, false = audioguia, true = guia
	bool enEspera = true;                     //Guarda en el hilo el valor de salaEspera(Indica si entra o no a la sala de espera)
	bool enEsperaGuia = true;                 //Indica que el visitante esta esperando pro un guia
	bool PRI = numRamdon;                     //Indica si el visitante es nuevo o no
	int tiempoUso;							  //Tiempo del recorrido
	int guiando;                              //Indica el # del guia del visitante
	int audioGuiaAdquirido;                   //ID del audio guia adquirido en recepcion
	

	//Se imprime en pantalla el ID del visitante y el recepcionista
	printf(" Visitante: %i en cola con recepcionista: %i\n", idVisitante, semCola_id(&sRecept[idHilo]));  

	semCola_wait(&sRecept[idHilo]);   //El visitante se forma en la cola del recepcionista #i
		while(!ban[idHilo]);                // Espera hasta que el hilo recepcionista que atiende al visitante se active

		/*Guarda los datos del visitante en el buffer del recepcionista*/
		tipoVisita[idHilo] = tipoGuia;      //Se guarada el tipo de visita (Guia/AudioGui)
		visitaTerminada[idHilo] = false;    //Indica que el visitante esta entrandos
		ID_hilo[idHilo] = idVisitante;      //Guarda el ID del hilo cliente
		prioridadGuia[idHilo] = PRI;        //Indica si el cliente es VIP
		
		pthread_mutex_lock(&mVisit[idHilo]);
			pthread_mutex_lock(&atencionCliente[idHilo]);
				banderaRecep[idHilo] = 1;
				pthread_cond_signal(&condMVisit[idHilo]);      //Envia una señal a recepcionista
		pthread_mutex_unlock(&mVisit[idHilo]);
				while(!banderaVisit[idHilo]){
					pthread_cond_wait(&condAtencionCliente[idHilo], &atencionCliente[idHilo]); //Espera señal de recepcionista
				}
				banderaVisit[idHilo] = 0;
			pthread_mutex_unlock(&atencionCliente[idHilo]);

		/*Se guarda el resultado de recepcionista*/
		if(tipoGuia){
			enEsperaGuia = salaEsperaGuia[idHilo];
			tiempoUso = tiempoUsoV[idHilo];
			guiando = guiandoV[idHilo];
		}else{
			audioGuiaAdquirido = idAudioGuia[idHilo];
			enEspera = salaEspera[idHilo];      
		}
	semCola_signal(&sRecept[idHilo]);  //El visitante se va de la cola del recepcionista #i
	
	if(!diaTerminado || !enEspera || !enEsperaGuia){
		if(tipoGuia){
		    /*Entra a la sala de espera*/
			if(enEsperaGuia){
				/*Espera por un dispositivo de AudioGuia*/
				while( !( (frente(&colGuiaPrioridad) == idVisitante ) && guiaLibre ) && !diaTerminado);
				if(!diaTerminado){
					pthread_mutex_lock(&mutexGuiaDesencolar);
						tiempoUso = tiempoRecorrido;
						guiando = guiaSaliendo;
						
		    			banderaGuiaDesencolar = 1;
		    			pthread_cond_signal(&condGuiaDesencolar); //Envia señal a guia
		    		pthread_mutex_unlock(&mutexGuiaDesencolar);			
		    	}
			}
			
			if(!diaTerminado){
				printf(" Visitante %i siguiendo a guia %i\n", idVisitante, guiando);
				sleep(tiempoUso);
				
				/*El primer grupo en salir es el primero en llegar*/
				while(guiaEntrando != guiando); 
				
				semCola_wait(&mensGuia);               //Recepcionista pide permiso para buscar un AudioGuia
					/*Se guarda los datos del visitante en las variables de guia*/
					ID_HILO_GUIA = idVisitante;     //Se guarda el ID del cliente
					recorridoTerminado = true;         //Se guarda el estado de entrada (entrando)
					propina = rand() % 10;
							
					pthread_mutex_lock(&senalGuia);
						pthread_mutex_lock(&recibidoGuia);
							banderaSenalGuia = 1;
							pthread_cond_signal(&condSenalGuia);     //Envia señal a guia
					pthread_mutex_unlock(&senalGuia);
							while(!banderaSenalGuiaRecepcionista){
		    					pthread_cond_wait(&condSenalGuiaRecep, &recibidoGuia); //Espera señal de guia
							}
							banderaSenalGuiaRecepcionista = 0;
						pthread_mutex_unlock(&recibidoGuia);
				semCola_signal(&mensGuia);	       //Regres*/
			}else{
				printf(" Visitante: %i no consiguio guia, salio del museo\n", idVisitante);
				
				sem_wait(&reenvolsoGuia),
					ingresoTotal -= 20;
				sem_post(&reenvolsoGuia);
			}
		}else{
			/*Entra a la sala de espera*/
			if(enEspera){
				/*Espera por un dispositivo de AudioGuia*/
				while(!(frente(&colAudioGuia) == idVisitante && audioGuiaLibre) && !diaTerminado);
				if(!diaTerminado){
					pthread_mutex_lock(&mutexAudioGuiaDesencolar);
		    			banderaDesencolar = 1;
		    			audioGuiaAdquirido = audioGuiaUsado;
		    			pthread_cond_signal(&condAudioGuiaDesencolar); //Envia señal a audio guia
		    		pthread_mutex_unlock(&mutexAudioGuiaDesencolar);	
		    	}
			}
			
			if(!diaTerminado){
				tiempoUso = rand() % (2 * hora);
		
				printf(" Visitante: %i usando audio guia %i\n", idVisitante, audioGuiaAdquirido);
				sleep(tiempoUso);
				printf(" Visitante: %i devolviendo audio guia %i\n", idVisitante, audioGuiaAdquirido); 
				
				semCola_wait(&sRecept[idHilo]);         //Se forma en la cola del recepcionista #i
					/*Guarda los datos del visitante en el buffer del recepcionista*/
					tipoVisita[idHilo] = tipoGuia;      //Se guarda el tipo de visita (Guia/AudioGuia)
					visitaTerminada[idHilo] = true;     //Indica que el visitante se va del museo
					ID_hilo[idHilo] = idVisitante;   //Guarda el ID del hilo cliente
					
					
					devolviendoAudioGuia[idHilo] = audioGuiaAdquirido;
					tiempoAudioGuia[idHilo] = tiempoUso;
					
					
					pthread_mutex_lock(&mVisit[idHilo]);
						pthread_mutex_lock(&atencionCliente[idHilo]);
							banderaRecep[idHilo] = 1;
							pthread_cond_signal(&condMVisit[idHilo]);  //Envia señal a recepcionista
					pthread_mutex_unlock(&mVisit[idHilo]);
							while(!banderaVisit[idHilo]){
								pthread_cond_wait(&condAtencionCliente[idHilo], &atencionCliente[idHilo]);  //Espera señal de recepcionista
							}
							banderaVisit[idHilo] = 0;
						pthread_mutex_unlock(&atencionCliente[idHilo]);
				semCola_signal(&sRecept[idHilo]);       //El visitante se va de la cola del recepcionista #i		
			}else{
				printf(" Visitante: %i no consiguio guia, salio del museo\n", idVisitante); 
			}
		}
	}else{
		printf(" Visitante: %i salio del museo\n", idVisitante);
	}
	((id_visit*)ID)->idRecept = NULL;
	free(((id_visit*)ID)->idVisit);
	free(((id_visit*)ID));

	return NULL;
}         

#endif