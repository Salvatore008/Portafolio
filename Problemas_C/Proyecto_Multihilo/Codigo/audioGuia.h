#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include "cola.h"
#include <pthread.h>

#ifndef audioGuia_h
#define audioGuia_h

#ifndef nRecepcionista
#define nRecepcionista 15
#endif

#ifndef nAudioGuias
#define nAudioGuias 30
#endif


/*Mutex recepcionista-audio guia*/
extern pthread_mutex_t mutexAudioGuiaDesencolar;
extern pthread_mutex_t senalAudioGuia;
extern pthread_mutex_t senalAudioGuiaRecep;

/*Condiciones del mutex recepcionista-audio guia*/
extern pthread_cond_t condSenalAudioGuia;
extern pthread_cond_t condSenalAudioGuiaRecep;
extern pthread_cond_t condAudioGuiaDesencolar;

/*Bandera del mutex recepcionista-audio guia*/
extern int banderaDesencolar;
extern int banderaSenalRecepcionista;
extern int banderaSenalAudioGuia;

/*Variables recepcionista-visitante-audio guia*/
extern bool salaEspera[nRecepcionista]; //Indica si el cliente entra o no a la sala de espera(cola de espera por AudioGuia)

/*Variable visitante-audio guia*/
extern bool audioGuiaLibre;             //Sale un cliente de la sala de espera

/*Variable recepcionista-audio guia*/
extern bool audioGuiaActivado;          //Indica que el hilo AudioGuia se activo
bool audioGuiaDevuelto;                 //Indica si el cliente pide o devuelve AudioGuia
int ID_HILO;                            //Indica el ID del hilo cliente 
int ID_salaEspera;                      //Indica el indice de salaEspera


Cola colAudioGuia;                      //Cola de espera por AudioGuia

extern bool diaTerminado;

int idRecepcionista;
int audioGuiaUsado;
int idAudioGuia[nRecepcionista];
Cola audioGuiaDisponible;
int tiempoTotalAudioGuia[nAudioGuias];
int tiempoUsoAudioGuia;
int audioGuiaUtilizados[nAudioGuias];

void* audioGuiaFun(void* ID){
	crearCola(&colAudioGuia);
	crearCola(&audioGuiaDisponible);
	
	for(int i = 0; i < nAudioGuias; i++){
		encolar(&audioGuiaDisponible, i + 1);
		tiempoTotalAudioGuia[i] = 0;
		audioGuiaUtilizados[i] = 0;
	}
	
	printf(" Hilo audioGuia       ACTIVADO\n");
	
	pthread_mutex_lock(&senalAudioGuia);
	audioGuiaActivado = true;
	
	while(1){		
		/*Espera señal de recepcionista*/
		while(!banderaSenalAudioGuia){
    		pthread_cond_wait(&condSenalAudioGuia, &senalAudioGuia);
		}
		banderaSenalAudioGuia = 0;

		if(!audioGuiaDevuelto){
			printf("\n Visitante: %i pide AudioGuia\n AudioGui disponible: %i\n Cola de espera: %i\n", ID_HILO, tam(&audioGuiaDisponible), tam(&colAudioGuia));
			if(nAudioGuias == 0){
				printf(" Visitante %i en cola\n\n", ID_HILO);
			}
		}else{
			printf("\n Visitante: %i devuelve AudioGuia %i\n AudioGui disponible: %i\n Cola de espera: %i\n", ID_HILO, audioGuiaUsado, tam(&audioGuiaDisponible) + 1, tam(&colAudioGuia));
		}
		
		if(audioGuiaDevuelto){
			tiempoTotalAudioGuia[audioGuiaUsado - 1] += tiempoUsoAudioGuia;		
			/*El visitante devuelve un audio guia*/
			if(tam(&colAudioGuia) > 0 && !diaTerminado){	
				audioGuiaUtilizados[audioGuiaUsado - 1] = 1;
				/*Se saca un visitante de la sala de espera*/
				pthread_mutex_lock(&mutexAudioGuiaDesencolar);
	   				audioGuiaLibre = true;
	   				while(!banderaDesencolar){
	   					pthread_cond_wait(&condAudioGuiaDesencolar, &mutexAudioGuiaDesencolar);	
					}
					audioGuiaLibre = false;
					banderaDesencolar = 0;
	   			pthread_mutex_unlock(&mutexAudioGuiaDesencolar);
				desencolar(&colAudioGuia);
			}else{
				audioGuiaUtilizados[audioGuiaUsado - 1] = 2;
				/*Aumenta el nuemro de audio guias disponibles*/
				encolar(&audioGuiaDisponible, audioGuiaUsado);
			}
		}else if(tam(&audioGuiaDisponible) > 0){
			/*El visitante obtiene un audio guia*/
			salaEspera[ID_salaEspera] = false;
			
			idAudioGuia[idRecepcionista] = frente(&audioGuiaDisponible);
			audioGuiaUtilizados[frente(&audioGuiaDisponible) - 1] = 1;
			desencolar(&audioGuiaDisponible);
				
		}else{
			/*El visitante entra a la sala de espera*/
			salaEspera[ID_salaEspera] = true;
			encolar(&colAudioGuia, ID_HILO);
		}	
	/*Envia señal a recepcionista*/
	pthread_mutex_lock(&senalAudioGuiaRecep);
		banderaSenalRecepcionista = 1;
		pthread_cond_signal(&condSenalAudioGuiaRecep);
	pthread_mutex_unlock(&senalAudioGuiaRecep);	
	}
		
	return NULL;
}

#endif