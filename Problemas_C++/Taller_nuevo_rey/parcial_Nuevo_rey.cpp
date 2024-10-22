#include <iostream>
#include <cstring>
#include "Arbol_bin_parcial_nuevo_rey.hpp"

using namespace std;

void borrar_espacios_linea();
void borrar_espacios_persona();

struct person{
	char let;
	char num;
}per;

int main(){
	list<person> l1; list<person> l2; list<person> l2_copia; 
	list<int> l3, l4;
	vector<int> v1;
	char orden_1[10];
	char orden_2[10];
	char car;
	int num_casos, num_familias, family_size, cont = 1, suma, mayor;
	bool ban = true;
	
	
	Arbol_bin<person> arbol = Arbol_bin<person>();
	
	cin >> num_casos; 
	
	while(num_casos-- > 0){
		cin >> num_familias;
		
		while(num_familias-- > 0){
			cin >> family_size;
			borrar_espacios_linea();
			
			cin.getline(orden_1, 10, ' ');
			while(ban){	
				borrar_espacios_persona();
				
				cin.get(per.let);
				
				l1.push_back(per);	
				
				if(cin.peek() == '\n'){
					ban = false;
				}
			}
			ban = true; cin.get(car);
			
			cin.getline(orden_2, 10, ' ');
			while(ban){	
				borrar_espacios_persona();
				
				cin.get(per.let);
				per.num = cont;
				cont++;
				
				l2.push_back(per);	
				
				if(cin.peek() == '\n'){
					ban = false;
				}
			}
			ban = true; cin.get(car);
			
			while(ban){
				cin >> cont;
				l3.push_back(cont);
				
				if(cin.peek() == '\n'){
					ban = false;
				}
			}
			v1.assign(l3.begin(), l3.end());
			l3.clear();
			cont = 1; ban = true; cin.get(car);
			
			l2_copia = l2;
			
			while(!l2_copia.empty()){
				if(l2_copia.front().let == l1.front().let){
					l1.front().num = l2_copia.front().num;
					l2_copia.pop_front();
					
				}
				l1.push_back(l1.front());
				l1.pop_front();
			}
			
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
			
			suma = arbol.nuevo_rey(v1);
			cout << "suma: " << suma << endl << endl;	
			l4.push_back(suma);
			
			v1.clear(); l1.clear(); l2.clear();		
		}
		
		mayor = l4.front();
		while(!l4.empty()){
			if(mayor < l4.front()){
				mayor = l4.front();
			}
			l4.pop_front();
		}
		cout << "Mayor: " << mayor << endl << endl;
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