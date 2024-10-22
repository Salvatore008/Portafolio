#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include "cola.h"

#ifndef nGuias
#define nGuias 10
#endif

#ifndef nGrupo
#define nGrupo 10
#endif

#ifndef nRecepcionistas
#define nRecepcionistas 15
#endif

#ifndef guia_h
#define guia_h

/*Mutex recepcionista-guia-visitante*/
extern pthread_mutex_t senalGuia;    
extern pthread_mutex_t recibidoGuia; 
extern pthread_mutex_t mutexGuiaDesencolar;

/*Condiciones del mutex recepcionista-guia-visitante*/
extern pthread_cond_t condSenalGuia;
extern pthread_cond_t condSenalGuiaRecep;
extern pthread_cond_t condGuiaDesencolar;

/*Bandera del mutex recepcionista-guia-visitante*/
extern int banderaSenalGuiaRecepcionista;
extern int banderaSenalGuia;
extern int banderaGuiaDesencolar;

extern bool guiaActivado;                    //Indica si el hilo inicio
extern bool guiaLibre;						 //Indica si hay un guia libre
extern bool salaEsperaGuia[nRecepcionistas]; //Indica si el cliente entra o no a la sala de espera(cola de espera por guia)
 
extern int nGuiasDisponibles;                //Numero de guias disponibles

/*Variables recepcionista-visitante-guia*/
bool recorridoTerminado; //Indica si el visitante esta entregando propina o no
int ID_HILO_GUIA;        //Identificador del visitante
int guiaSaliendo;        //Indica el proximo guia en salir a dar el recorrido
int tiempoRecorrido;     //Tiempo de duracion del recorrido


/*Variables recepcionista-guia*/
bool prioridad;          //Indica si el visitante es prioritario o no
int ID_salaEsperaGuia;   //Indice parar el vector salaEsperaGuia
         
/*Variables visitante-guia*/
int propina;             //Propina del visitante #i
int guiaEntrando;        //Guia que termino el recorrido

int grupo;               //Numero de visitantes que han entregado propina
int nPropinas[nGuias];   //Cantidad de propina del guia #i
                         
Cola colGuia;            //Cola de espera 
Cola colGuiaPrioridad;   //Cola de espera prioritaria

extern int numGuiasActivosHora;
int duracionRecorridos[nGuias];

extern long long hora; 

void* guia(void* ID){
	int i;
	guiaEntrando = 1;
    guiaSaliendo = 1;
    grupo = 0;
	
	crearCola(&colGuia);
	crearCola(&colGuiaPrioridad);
	
	for(i = 0; i < nGuias; i++){
		nPropinas[i] = 0;
		duracionRecorridos[i] = 0;
	}
	
	printf(" Hilo Guia            ACTIVADO\n\n");
	pthread_mutex_lock(&senalGuia);
	guiaActivado = true;
	
	while(1){
		/*Espera una señal de recepcionista o vistante*/
		while(!banderaSenalGuia){
			pthread_cond_wait(&condSenalGuia, &senalGuia);
		}
		banderaSenalGuia = 0;
		
			if(recorridoTerminado){
				grupo++;
				nPropinas[guiaEntrando - 1] += propina;
				
				if(grupo == 1){
					printf("\n");
				}
				printf(" Visitante %i dio %i de propina a guia %i\n", ID_HILO_GUIA, propina, guiaEntrando);
				
				
				if(grupo == nGrupo){
					printf("\n");
					
					grupo = 0;
					nGuiasDisponibles++;
					guiaEntrando++;

					if(guiaEntrando > nGuias){
						guiaEntrando = 1;	
					}
					
					if((tam(&colGuia) + tam(&colGuiaPrioridad) >= nGrupo)){
						if(numGuiasActivosHora < nGuias){
							numGuiasActivosHora++;
						}
						
						salaEsperaGuia[ID_salaEsperaGuia] = false;
						nGuiasDisponibles--;
							
						/*se rellena el grupo de salida con los visitantes regulares si es necesario*/
						while(tam(&colGuiaPrioridad) < nGrupo){
							encolar(&colGuiaPrioridad, frente(&colGuia));
							desencolar(&colGuia);
						}
							
						printf("\nGuia %i saliendo\n", guiaSaliendo);					
							
						tiempoRecorrido = (rand() % (hora - (hora - 3) + 1)) + (hora - 3);
						duracionRecorridos[guiaSaliendo - 1] += tiempoRecorrido;
							
						/*se sacan los visitantes de la sala de espera*/
						pthread_mutex_lock(&mutexGuiaDesencolar);
				    		guiaLibre = true;
				    		for(i = 0; i < nGrupo; i++){
				    			while(!banderaGuiaDesencolar){
				    				pthread_cond_wait(&condGuiaDesencolar, &mutexGuiaDesencolar);	
								}
								banderaGuiaDesencolar = 0;
								if(i < nGrupo - 1){
									desencolar(&colGuiaPrioridad);
								}		
							}			
							guiaLibre = false;		
							desencolar(&colGuiaPrioridad);
				    	pthread_mutex_unlock(&mutexGuiaDesencolar);
				
				    	/*se actualiza el proximo guia en salir*/
						guiaSaliendo++;
						if(guiaSaliendo > nGuias){
							guiaSaliendo = 1;
						}
					}
				}
			}else if(nGuiasDisponibles > 0 && (tam(&colGuia) + tam(&colGuiaPrioridad) + 1 >= nGrupo )){
				if(numGuiasActivosHora < nGuias){
					numGuiasActivosHora++;
				}
				
				salaEsperaGuia[ID_salaEsperaGuia] = false;
				nGuiasDisponibles--;
				
				/*se rellena el grupo de salida con los visitantes regulares si es necesario*/
				while(tam(&colGuiaPrioridad) < nGrupo - 1){
					encolar(&colGuiaPrioridad, frente(&colGuia));
					desencolar(&colGuia);
				}
				
				printf("\nGuia %i saliendo\n", guiaSaliendo);
				
				tiempoRecorrido = (rand() % (hora - (hora - 3) + 1)) + (hora - 3);
				duracionRecorridos[guiaSaliendo - 1] += tiempoRecorrido;
				
				/*se sacan los visitantes de la sala de espera*/
				pthread_mutex_lock(&mutexGuiaDesencolar);
	    			guiaLibre = true;
	    			for(i = 0; i < nGrupo - 1; i++){
	    				while(!banderaGuiaDesencolar){
	    					pthread_cond_wait(&condGuiaDesencolar, &mutexGuiaDesencolar);	
						}
						banderaGuiaDesencolar = 0;
						if(i < nGrupo - 2){
							desencolar(&colGuiaPrioridad);
						}		
					}			
					guiaLibre = false;		
					desencolar(&colGuiaPrioridad);
	    		pthread_mutex_unlock(&mutexGuiaDesencolar);
	    		
	    		/*se actualiza el proximo guia en salir*/
				guiaSaliendo++;
				if(guiaSaliendo > nGuias){
					guiaSaliendo = 1;
				}
			}else{
				salaEsperaGuia[ID_salaEsperaGuia] = true;
				printf(" Visitante %i en espera por guia\n", ID_HILO_GUIA);
					
				/*Ingresa el visitante a la sala de espera*/
				if(prioridad){
					encolar(&colGuiaPrioridad, ID_HILO_GUIA);
				}else{
					encolar(&colGuia, ID_HILO_GUIA);
				}		
			}
		/*Envia una señal a recepcionista o visitante*/
		pthread_mutex_lock(&recibidoGuia);
			banderaSenalGuiaRecepcionista = 1;
			pthread_cond_signal(&condSenalGuiaRecep);
		pthread_mutex_unlock(&recibidoGuia);
	}
	
	return NULL;
}

#endif