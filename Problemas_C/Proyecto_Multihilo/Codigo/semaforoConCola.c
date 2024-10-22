#include "semaforoConCola.h"
#include <stdio.h>
#include <stdlib.h>

/*Se inicializa el semaforo*/
void* semCola_init(semCola* sc, int ID, int valorInit){
	sem_init(&sc->sem, 0, valorInit);
	sc->id = ID;
	sc->cola = NULL;
}

/*Se encola un nodo en la cola, similar a una lista*/
void* semCola_wait(semCola* sc) {
    Nodo* nodoActual = (Nodo*)malloc(sizeof(Nodo));
    nodoActual->hilo = pthread_self();
    nodoActual->sig = NULL;

    sem_wait(&sc->sem);
    if (sc->cola == NULL) {
        sc->cola = nodoActual;
    } else {
        Nodo* nodo = sc->cola;
        while (nodo->sig != NULL){
            nodo = nodo->sig;
        }
        nodo->sig = nodoActual;
    }
    sem_post(&sc->sem);

    while (sc->cola != nodoActual);// Esperar en la cola hasta que sea el turno del hilo actual
}

/*Se desencola del semaforo*/
void* semCola_signal(semCola* sc) {
    sem_wait(&sc->sem);

    if(sc->cola != NULL) {
        Nodo* nodo = sc->cola;
        sc->cola = nodo->sig;
        free(nodo);        
    }
    sem_post(&sc->sem);
}

/*Se retorna el ID del semaforo*/
int semCola_id(semCola* sem){
	return sem->id;
}

/*Se destruye el semaforo*/
void* semCola_destroy(semCola* sc){
	sem_destroy(&sc->sem);
}