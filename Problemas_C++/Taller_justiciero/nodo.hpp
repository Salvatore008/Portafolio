#ifndef nodo_h
#define nodo_h

/*
Autor: Luigi Quero C.I: 30.009.785
*/

template <class E>
class Nodo{
	private:
		E elem;
		Nodo<E>* sig;
	public:
		Nodo(E _elem);
		Nodo(E _elem, Nodo<E>* _sig);
		void set_elem(E _elem);
		void set_sig(Nodo<E>* sig);
		E get_elem();
		Nodo<E>* get_sig();
};

template <class E>
Nodo<E> :: Nodo(E _elem){
	elem = _elem;
	sig = 0;
}

template <class E>
Nodo<E> :: Nodo(E _elem, Nodo<E>* _sig){
	elem = _elem;
	sig = _sig;
}

template <class E>
void Nodo<E> :: set_elem(E _elem){
	elem = _elem;
}

template <class E>
void Nodo<E> :: set_sig(Nodo<E>* _sig){
	sig = _sig;
}

template <class E>
E Nodo<E> :: get_elem(){
	return elem;
}

template <class E>
Nodo<E>* Nodo<E> :: get_sig(){
	return sig;
}

#endif
