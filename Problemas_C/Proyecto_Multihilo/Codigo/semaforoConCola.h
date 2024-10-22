#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#ifndef semaforoConCola_h
#define semaforoConCola_h


 typedef struct Nodo{
	pthread_t hilo;
	struct Nodo* sig;
}Nodo;

 typedef struct {
	int id;
	sem_t sem;
	Nodo* cola;
}semCola;

/*Estructura que se pasa como argumento a los hilos*/
typedef struct{
	int id;   
}id_h;

/*Estructura que se pasa como argumento a los hilos*/
typedef struct{
	id_h* idRecept;
	int*  idVisit;
}id_visit;


void* semCola_init(semCola* sem, int ID, int valorInit);
void* semCola_wait(semCola* sem);
void* semCola_signal(semCola* sem);
int semCola_id(semCola* sem);
void* semCola_destroy(semCola* sem);

#endif