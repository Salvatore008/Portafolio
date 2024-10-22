#include "DireccionMuseo.h"

int main(){
	id_h hilo[nRecepcionista];     //ID de los hilos usada como indice en los vectortes semaforos y banderas
	id_visit* idHiloVisit;
	pthread_t audioGuiaHilo;       //Hilo AudioGuia
	pthread_t guiaHilo;            //Hilo guia
	pthread_t timeHilo;			   //Hilo de time 
	long long nVisits;             //Visitantes totales del museo
	
	audioGuiaActivado = false;         
	guiaActivado = false;
	audioGuiaLibre = false;
	guiaLibre = false;
	diaTerminado = false;
	
	nGuiasDisponibles = 5;
	
	/*Banderas de audio guia*/
	banderaDesencolar = 0;
	banderaSenalRecepcionista = 0;
    banderaSenalAudioGuia = 0;
    /*Banderas de guia*/
    banderaSenalGuiaRecepcionista = 0;
    banderaSenalGuia = 0;
    banderaGuiaDesencolar = 0;
    
    /*Mutex recepcionista-audio guia*/
    pthread_mutex_init(&senalGuia, NULL);
    pthread_mutex_init(&recibidoGuia, NULL);
    pthread_mutex_init(&mutexGuiaDesencolar, NULL);
    
    pthread_cond_init(&condSenalGuia, NULL);
    pthread_cond_init(&condSenalGuiaRecep, NULL);
    pthread_cond_init(&condGuiaDesencolar, NULL);
    
    /*Mutex recepcionista-guia*/
    pthread_mutex_init(&senalAudioGuia, NULL);
    pthread_mutex_init(&senalAudioGuiaRecep, NULL);
    pthread_mutex_init(&mutexAudioGuiaDesencolar, NULL);
    
    pthread_cond_init(&condSenalAudioGuia, NULL);
    pthread_cond_init(&condSenalAudioGuiaRecep, NULL);
    pthread_cond_init(&condAudioGuiaDesencolar, NULL);

	/*Cola recepcionista-audio guia*/
	semCola_init(&mensAudioGuia, 0 , 1);     //Incia el semaforo en 1
	/*Cola recepcionista-guia*/
	semCola_init(&mensGuia, 0, 1);           //Incia el semaforo en 1
	/*Cola de espera por reenvolso no conseguir guia*/
	sem_init(&reenvolsoGuia, 0, 1);          //Inicia el semaforo en 1
	
	
	printf(" EL museo abre desde las 7am-5pm (10 horas)\n");
	printf(" Ingrese la duracion de una hora en seg(un dia son 10 horas): "); scanf("%lld", &hora);    //se lee la duracion del dia
	hora += 3;
	printf("\n Ingrese el numero de visitantes: "); scanf("%lld", &nVisits); //Se lee el numero de visitantes del museo
	long long* visits = malloc(nVisits * sizeof(long long));

	/*Verifica que el numero se halla leido correctamente*/
	if (visits == NULL) {
        printf(" Error al asignar memoria para el vector de hilos\n");
        return 1;
    }
    
    printf("\nActivando hilos:\n\n");
    
	/*Se inicia los hilos recepcionistas*/
	for(int i = 0; i < nRecepcionista; i++){	
		/*Mutex recepcionista-visitante*/
		pthread_mutex_init(&atencionCliente[i], NULL);
		pthread_mutex_init(&mVisit[i], NULL);
		
		pthread_cond_init(&condMVisit[i], NULL);
		pthread_cond_init(&condAtencionCliente[i], NULL);
		
		hilo[i].id = i;                    //Guarda el ID del recepcionista en el vector
		ban[i] = false;                    //Se coloca la bandera del recepcionista #i como falso
		
		/*Cola recepcionista-visitante*/
		semCola_init(&sRecept[i], i + 1,  1);  //Se inicia la cola del semaforo del recepcionista #i en 1
			
		pthread_create(&recepts[i], NULL, recept, &hilo[i]);  //Se crea el hilo recepcionista
	}
	
	pthread_create(&audioGuiaHilo, NULL, audioGuiaFun, NULL); //Se crea el hilo AudioGuia
	pthread_create(&guiaHilo, NULL, guia, NULL); //Se crea el hilo AudioGuia
	pthread_create(&timeHilo, NULL, timeF, &hora);
	
	int visitantesDelDia = 0;
	/*Se inician los hilos visitantes*/
	for(int i = 0; i < nVisits && !diaTerminado; i++){
		visitantesDelDia++;
		idHiloVisit = malloc(nVisits * sizeof(id_visit));
		idHiloVisit->idRecept = &hilo[i % nRecepcionista];
		idHiloVisit->idVisit = (int*)malloc(nVisits * sizeof(int));
		*(idHiloVisit->idVisit) = i + 1;
		//Se crean los hilos recepcionistas (se asginan los visitantes a los recepcionista en orden circular)
		pthread_create((pthread_t*)&visits[i], NULL, visit, idHiloVisit);  
		idHiloVisit = NULL;
		sleep(1);
	}
	
    /*Espera hasta que todos los hilos visitantes terminen su ejecución*/
    for(int i = 0; i < visitantesDelDia; i++) {
        pthread_join(visits[i], NULL);
    }
	pthread_join(timeHilo, NULL);
	
	printf("\n");
	for(int i = 0; i < nGuias; i++){
		printf(" Duracion de los recorridos del guia %i: %i\n", i + 1, duracionRecorridos[i]);
	}
	
	printf("\n");
	for(int i = 0; i < nGuias; i++){
		printf(" Propina del guia %i: %i\n", i + 1, nPropinas[i]);
	}
	
	printf("\n");
	for(int i = 0; i < nAudioGuias; i++){
		printf(" Tiempo de uso del audio guia %i: %lli\n", i + 1, tiempoTotalAudioGuia[i] / hora);
	}
	
	
	for(int i = 0; i < nRecepcionista; i++){
		ingresoTotal += ingresoRecepcion[i];
	}
	printf("\n Ingreso del dia: %i", ingresoTotal);
	printf("\n----------------------------------------------------------\n");
    

	return 0;
}