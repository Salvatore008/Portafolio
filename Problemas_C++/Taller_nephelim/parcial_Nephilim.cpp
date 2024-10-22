#include <iostream>
#include <cstring>
#include "Arbol_bin_parcial_nephelim.hpp"

using namespace std;

void borrar_espacios_linea();
void borrar_espacios_persona();

struct person{
	char nombre[30];
	char gen;
}per, per1, per2;

int main(){
	list<person> l1; list<person> l2;
	char nombre[30];
	char orden_1[10];
	char orden_2[10];
	char car;
	int num_casos, nep = 0, dist, num_corrida = 1;
	bool ban = true;
	
	strcpy(per1.nombre, "harold");
	per1.gen = 'M';
	
	strcpy(per2.nombre, "alvaro");
	per2.gen = 'N';
	
	Arbol_bin<person> arbol = Arbol_bin<person>();
	
	cin >> num_casos; 
	
	while(num_casos-- > 0){
		cout << endl <<"Corrida " << num_corrida++ << "------------------------------------------------------------" << endl << endl;
		borrar_espacios_linea();
		
		cin.getline(orden_1, 10, ' ');
		while(ban){	
			borrar_espacios_persona();
			cin.getline(per.nombre, 30, '-');
			cin.get(per.gen);
			
			l1.push_back(per);	
			
			if(cin.peek() == '\n'){
				ban = false;
			}
		}
		ban = true;
		cin.get(car);
		
		cin.getline(orden_2, 10, ' ');
		while(ban){
			borrar_espacios_persona();
			cin.getline(per.nombre, 30, '-');
			cin.get(per.gen);
			
			l2.push_back(per);
		
			if(cin.peek() == '\n' || cin.peek() == EOF){
				ban = false;
			}
		}
		ban = true;
		cin.get(car);
		
		if(orden_1[1] == 'R'){
			arbol.crear_in_pre(l2, l1);
		}else{
			if(orden_2[1] == 'R'){
				arbol.crear_in_pre(l1, l2);
			}
		}
		
		if(orden_1[1] == 'O'){
			arbol.crear_in_post(l2, l1);
		}else{
			if(orden_2[1] == 'O'){
				arbol.crear_in_post(l1, l2);
			}
		}
		l1.clear(); l2.clear();
		
		l1 = arbol.pre_orden();
		cout << "Pre Orden: ";
		while(!l1.empty()){
			cout << l1.front().nombre << " ";
			l1.pop_front();
		}
		cout << endl;
		
		l1 = arbol.in_orden();
		cout << "In Orden: ";
		while(!l1.empty()){
			cout << l1.front().nombre << " ";
			l1.pop_front();
		}
		cout << endl;
		
		l1 = arbol.post_orden();
		cout << "Post Orden: ";
		while(!l1.empty()){
			cout << l1.front().nombre << " ";
			l1.pop_front();
		}
		cout << endl << endl;
		
		l2= arbol.camino_nod_nod(per1, per2);
		cout << "Camino al nodo: ";
		while(!l2.empty()){
			cout << l2.front().nombre << " ";
			l2.pop_front();
		}
		cout << endl << endl;
		
		l1 = arbol.camino_nod_nod(per1, per2);
		dist = l1.size() - 1;
		
		cout << "Camino de nodo al nodo: ";
		while(!l1.empty()){
			cout << l1.front().nombre << " ";
			if(l1.front().gen == 'N'){
				nep++;
			}
			l1.pop_front();
		}
		cout << endl << "Distancia entre nodos: " << dist << endl;
		cout << "Numero de nephelim: " << nep << endl;
		cout << "NSE (Nephilim Strength Estimator: " << static_cast<float>(nep) / dist << endl;
		
		nep = 0; dist = 0;
		
		
		
		arbol.~Arbol_bin<person>();
	}
	
	return 0;
}

void borrar_espacios_linea(){
	char aux;
	while(cin.peek() != '\n'){
		cin.get(aux);
	}
	cin.get(aux);
}

void borrar_espacios_persona(){
	char aux;
	while(cin.peek() == ' '){
		cin.get(aux);
	}
}