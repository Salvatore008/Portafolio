#include <stdio.h>
#include <stdlib.h>

#ifndef cola_h
#define cola_h

typedef struct Nod{
	int valor;
	struct Nod* prox;
}Nod;

typedef struct Cola{
	Nod* prim;
	Nod* ultm;
	int size;
}Cola;

void crearCola(Cola* c);
void encolar(Cola* c, int valor);
void desencolar(Cola* c);
int frente(Cola* c);
int tam(Cola* c);

#endif
