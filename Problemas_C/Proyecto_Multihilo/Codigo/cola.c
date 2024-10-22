#include "cola.h"


void crearCola(Cola* c){
	c->prim = NULL;
	c->ultm = NULL;
	c->size = 0;
}

void encolar(Cola* c, int valor){
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->valor = valor;
	nod->prox = NULL;
	
	if(c->size == 0){
		c->prim = nod;
		c->ultm = nod;
	}else{
		c->ultm->prox = nod;
		c->ultm = nod;
	}
	c->size++;
}
void desencolar(Cola* c){
	Nod* nod;
	
	if(c->size > 0){
		nod = c->prim;
		c->prim = nod->prox;
		free(nod);
		c->size--;
	}	
}

int frente(Cola* c){
	if(c->size > 0){
		return c->prim->valor;	
	}else{
		return -1;
	}
}

int tam(Cola* c){
	return c->size;	
}