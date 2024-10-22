#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "cola.h"

#ifndef time_h
#define time_h

#ifndef nRecepcionista
#define nRecepcionista 15
#endif

#ifndef nGuias 
#define nGuias 10
#endif

#ifndef nAudioGuias 
#define nAudioGuias 30
#endif

#define dia 10


extern bool diaTerminado;

extern pthread_mutex_t senalGuia;    
extern pthread_mutex_t mVisit[nRecepcionista];

extern int numVisitantesHora[nRecepcionista];
extern int numGuiasActivosHora;
extern int nPropinas[nGuias];
extern int audioGuiaUtilizados[nAudioGuias];

extern Cola colAudioGuia; 
extern Cola colGuia;                     
extern Cola colGuiaPrioridad; 

void* timeF(void* ID){
	int visitantes_dia = 0;
	int visitantes_hora = 0;
	int propinas_guia_dia = 0;
	int audioGuiasActivosHora;
	
	
	for(int j = 0; j < dia; j++){
		sleep(*(long long*)ID);
		
		visitantes_hora = 0;
		for(int i = 0; i < nRecepcionista; i++){
			pthread_mutex_lock(&mVisit[i]);
			visitantes_hora += numVisitantesHora[i];
		}
		visitantes_dia += visitantes_hora;
		
		audioGuiasActivosHora = 0;
		for(int i = 0; i < nAudioGuias; i++){
			if(audioGuiaUtilizados[i] >= 1){
					audioGuiasActivosHora++;
			}
			if(audioGuiaUtilizados[i] == 2){
				audioGuiaUtilizados[i] = 0;
			}
		}
		
		if(j != 9){
			pthread_mutex_lock(&senalGuia);
				printf("\n----------------------Hora %i terminada----------------------\n", j + 1);
				printf(" Visitantes por hora: %i\n", visitantes_hora);
				printf(" Guias activos por hora: %i\n", numGuiasActivosHora);
				printf(" Visitantes en la sala de espera: %i\n", tam(&colAudioGuia) + tam(&colGuia) + tam(&colGuiaPrioridad));
				printf(" Audio guias utilizados: %i - audio guias libres: %i\n", audioGuiasActivosHora, nAudioGuias - audioGuiasActivosHora);
				printf("\n----------------------------------------------------------\n");
			pthread_mutex_unlock(&senalGuia);
		}else{	
			pthread_mutex_lock(&senalGuia);
				printf("\n----------------------Hora %i terminada----------------------\n", j);
				printf(" Visitantes por hora: %i\n", visitantes_hora);
				printf(" Guias activos por hora: %i\n", numGuiasActivosHora);
				printf(" Visitantes en la sala de espera: %i\n", tam(&colAudioGuia) + tam(&colGuia) + tam(&colGuiaPrioridad));
				printf(" Audio guias utilizados: %i - audio guias libres: %i\n", audioGuiasActivosHora, nAudioGuias - audioGuiasActivosHora);
				printf("\n----------------------------------------------------------\n");
			pthread_mutex_unlock(&senalGuia);
			
			diaTerminado = true;
			printf("\n----------------------Dia terminada------------------------\n");
			printf(" Visitantes por dia: %i\n", visitantes_dia);
		}
		
		
		numGuiasActivosHora = 0;
		for(int i = 0; i < nRecepcionista; i++){
			pthread_mutex_unlock(&mVisit[i]);
			numVisitantesHora[i] = 0;
		}
	}
}

#endif