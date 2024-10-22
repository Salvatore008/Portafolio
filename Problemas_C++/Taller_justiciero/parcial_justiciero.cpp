#include <iostream>
#include "lista.hpp"

using namespace std;

struct Persona;

void anuncio();
void ruleta(Lista<Persona>& list, int cont, int prioridad_casta);
void imprimir_lista(Lista<Persona> list);
bool menor_casta(Lista<Persona> list, int casta);
bool mayor_casta(Lista<Persona> list, int casta);

struct Persona{
	char nombre[30];
	int casta;
} per;

int main(){
	Lista<Persona> list = Lista<Persona>();
	int juego = 0, _juego = 1, _casta = 0, cont = 0, prioridad_casta = 0;
	char car = 0;
	
	anuncio();
	
	cin >> juego;
	
	while(juego-- > 0){
		cin >> _casta >> cont >> prioridad_casta;
		cout << "Juego: " << _juego++ << endl;
		
		cin >> car;
		while(car != ']'){
			cin >> car;
			
			if(car == '('){
				cin.getline(per.nombre, 30, ',');
				cin >> per.casta;
				list.insertar(per, list.longitud() + 1);
			}
		}
		
		ruleta(list, cont, prioridad_casta);	
	}
	

    return 0;
}

void anuncio(){
	cout << "\n\n----------Bienvenidos a los juegos del hambre!!, edicion PaPaCaliente\n" << endl;
}


void ruleta(Lista<Persona>& list, int cont, int prioridad_casta){
	int cont_actual = 1, cont_muertes = 0, ronda = 1;
	bool band = false;
	
	while(list.longitud() > 1){
		
		if(cont_muertes == prioridad_casta){
			band = !band;
			cont_muertes = 0;
		}
		
		if(cont_actual == cont){
			cont_actual = 0;
			
			if( (band && menor_casta(list, list.consultar(1).casta)) || (!band && mayor_casta(list, list.consultar(1).casta)) ){
				cout << "	Ronda:" << ronda <<  "	Prioridad a la casta alta: " << !band << "\n\n		Eliminado: " << list.consultar(1).nombre << " Casta: " << list.consultar(1).casta << "\n\n";
				list.eliminar(1);
				cont_muertes++; ronda++;
				
				imprimir_lista(list);	
			}
					
		}else{
			list.insertar(list.consultar(1), list.longitud() + 1);
			list.eliminar(1);
		}
		
		cont_actual++;
	}
	
	cout << " Felicidades " << list.consultar(1).nombre << "!! te has ganado tu vida, reza para que no nos volvamos a ver; JAJAJA!!!\n\n\n" << endl;
	list.eliminar(1);
}

void imprimir_lista(Lista<Persona> list){
	while(!list.es_vacia()){
		cout << "		Nombre: " << list.consultar(1).nombre << " Casta: " << list.consultar(1).casta << endl;
		list.eliminar(1);
	}
	cout << endl;
}


bool menor_casta(Lista<Persona> list, int casta){
	bool ban = true;
	
	while(!list.es_vacia() && ban){
		if(list.consultar(1).casta < casta){
			return false;
		}
		
		list.eliminar(1);
	}
	
	return true;
}

bool mayor_casta(Lista<Persona> list, int casta){
	bool ban = true;
	
	while(!list.es_vacia() && ban){
		if(list.consultar(1).casta > casta){
			return false;
		}
		
		list.eliminar(1);
	}
	
	return true;
}
