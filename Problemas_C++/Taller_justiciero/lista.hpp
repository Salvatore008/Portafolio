#ifndef lista_h
#define lista_h

/*
Autor: Luigi Quero C.I: 30.009.785
*/

#include "nodo.hpp"
#include <iostream>

using namespace std;

template <class E>
class Lista{
	protected:
		Nodo<E>* prim;
		Nodo<E>* ultm;
		int longt;
	public:
		Lista();
		~Lista();
		void insertar(E elem, int pos);
		void modificar(E elem, int pos);
		void eliminar(int pos);
		void invertir(); 
		void ordenar();
		void ruta_memoria(int& ini, int& fin);
		Lista<E> get_ordenar();
		Lista<E> get_invertir();
		Lista<E> copia();
		Lista<E> sub_lista(int prim, int ultm);
		E consultar(int pos);
		int longitud();		
		bool es_vacia();	
};


template <class E>
void Lista<E> :: ruta_memoria(int& ini, int& fin){
	Nodo<E>* p;
	Nodo<E>* p2;
	bool ban = false;
	
	if(!this->es_vacia()){
		p = this->prim;	p2 = p->get_sig();
		ini = 1; fin = 2;
		
		while(ini < this->longitud() && !ban){
			if(p->get_elem() == p2->get_elem()){
				ban = true;
			}else{
				p2 = p2->get_sig();
				fin = fin + 1;
				
				if(p2 == 0){
					p = p->get_sig();
					p2 = p->get_sig();
					ini = ini + 1;
					fin = ini + 1;					
				}
			}
		}
		
		if(fin > this->longitud()){
			ini = 0; fin = 0;
		}				
	}else{
		ini = 0; fin = 0;
	}
}

template <class E>
Lista<E> :: Lista(){
	prim = 0;
	ultm = 0;
	longt = 0;
}

template <class E>
Lista<E> :: ~Lista(){
	Nodo<E>* p = 0;
	
	while(longt-- > 0){
		p = prim;
		prim = prim->get_sig();
		p->~Nodo();
	}
	ultm = prim;
}

template <class E>
void Lista<E> :: insertar(E elem, int pos){
	Nodo<E>* p_nodo = new Nodo<E>(elem); Nodo<E>* p1 = prim; Nodo<E>* p2 = 0;
	
	if(longt == 0){
		prim = p_nodo;
		ultm = prim;
	}
	else{
		if(pos <= 1){
			p_nodo->set_sig(prim);
			prim = p_nodo;
		}
		else if(pos > longt){
			ultm->set_sig(p_nodo);
			ultm = p_nodo;
		}
		else{
			p2 = p1->get_sig();
			
			while(--pos > 1){
				p1 = p2; 
				p2 = p2->get_sig();
			}
			
			p_nodo->set_sig(p2);
			p1->set_sig(p_nodo);
		}
	}
	longt++;
}

template <class E>
void Lista<E> :: modificar(E elem, int pos){
	Nodo<E>* p_nodo = prim;

	if(longt != 0){
		if(pos <= 1){
			prim->set_elem(elem);
		}
		else if(pos >= longt){
			ultm->set_elem(elem);
		}
		else{
			while(--pos > 0){
				p_nodo = p_nodo->get_sig();
			}

			p_nodo->set_elem(elem);
		}
	}
}

template <class E>
void Lista<E> :: eliminar(int pos){
	Nodo<E>* p1 = prim;
	Nodo<E>* p2 = 0;
	
	if(longt > 0){
		if(longt == 1){
			prim->~Nodo();
			prim = 0;
			ultm = 0;
		}
		else if(pos <= 1){
			prim = prim->get_sig();
			p1->~Nodo();
		}
		else{
			p2 = p1->get_sig();
			
			while(--pos > 1 && p2->get_sig() != 0){
				p1 = p2; 
				p2 = p2->get_sig();
			}
			
			if(p2 == ultm){
				ultm = p1;
			}
			
			p1->set_sig(p2->get_sig());
			p2->~Nodo();		
		}
		longt--;
	}
}

template <class E>
void Lista<E> :: invertir(){
	Nodo<E>* ant = 0;
    Nodo<E>* act = this->prim;
    Nodo<E>* sig = 0;

    while (act != 0) {
        sig = act->get_sig();
        act->set_sig(ant);
        ant = act;
        act = sig;
    }

    this->ultm = this->prim;
    this->prim = ant;
}


template <class E>
void Lista<E> :: ordenar(){
	Nodo<E>* p1; Nodo<E>* p2; Nodo<E>* p3;
	int lon = this->longt, cont = 0, cont2 = 0;
	
	this->ultm->set_sig(this->prim);
	this->prim = this->prim->get_sig();
	p3 = this->ultm->get_sig();
	p3->set_sig(0);
	
	while(lon-- > 1){
		cont++;
		if(this->prim->get_elem() < this->ultm->get_sig()->get_elem()){
			if(lon > 1){
				p1 = this->prim;
				this->prim = this->prim->get_sig();
				p1->set_sig(this->ultm->get_sig());
				this->ultm->set_sig(p1);
			}	
		}else{
			p1 = this->ultm->get_sig();
			p2 = p1->get_sig();
			
			while(p2 != 0 && this->prim->get_elem() > p2->get_elem()){
				p1 = p2;
				p2 = p2->get_sig();
				
				cont2++;
				if(cont2 > 0){
					cont2--;
					cont++;
				}
			}
				
			p1->set_sig(this->prim);
			this->prim = this->prim->get_sig();
			p1->get_sig()->set_sig(p2);
		
			if(p2 == 0){
				p3 = p1->get_sig();
			}	
		}
	}
	cout << cont << endl << endl;
	this->ultm = p3;
}

template <class E>
Lista<E> Lista<E> :: get_ordenar(){
	Lista<E> cop = this->copia();
	cop.ordenar();
	
	return cop;
}

template <class E>
Lista<E> Lista<E> :: get_invertir(){
	Lista<E> li = Lista<E>();
	int aux_longitud = longt;
	
	if(longt > 0){
		while(aux_longitud-- > 0){
			this->insertar(this->consultar(1), longt + 1);
			li.insertar(this->consultar(1), 1);
			this->eliminar(1);
		}
	}	

	return li;
}

template <class E>
Lista<E> Lista<E> :: copia(){
	Lista<E> list = Lista<E>();
	Nodo<E>* p = this->prim;
	
	while(p != 0){
		list.insertar(p->get_elem(), list.longitud() + 1);
		p = p->get_sig();
	}
	
	return list;
}

template <class E>
Lista<E> Lista<E> :: sub_lista(int _prim, int _ultm){
	Lista<E> aux = Lista<E>();
	Nodo<E>* nod = this->prim;

	if(_prim <= _ultm && _prim > 0 && _ultm <= this->longitud()){
		_ultm = _ultm + 1 - _prim;
	
		while(--_prim > 0){
			nod = nod->get_sig();
		}
		while(_ultm-- > 0){
			aux.insertar(nod->get_elem(), aux.longitud() + 1);
			nod = nod->get_sig();
		}	
	}

	return aux;
}

template <class E>
E Lista<E> :: consultar(int pos){
	Nodo<E>* p_nodo = prim;
	
	if(pos <= 1){
		return prim->get_elem();
	}
	else if(pos >= longt){
		return ultm->get_elem();
	}
	
	while(--pos > 0){
		p_nodo = p_nodo->get_sig();
	}
	
	return p_nodo->get_elem();
}

template <class E>
int Lista<E> :: longitud(){
	return longt;
}

template <class E>
bool Lista<E> :: es_vacia(){
	return (longt == 0);
}

#endif
