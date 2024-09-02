#include <iostream>
#include <queue>
#include <stdint.h>
using namespace std;

#define INT_MAX 0x7fff

bool pertenece(int* array,int n, int x){
	for(int i=0; i<n; i++){
		if(array[i]==x) return true;
	}
	return false;
}


/*
Estructura para las aristas del grafo
*/
struct arista {
	int datoOrigen;
    int datoDestino;
    int peso; 
    arista *sgteArista;
};
typedef arista *parista;


/*
Estructura para vertices del grafo
*/
struct vertice {
    int datoOrigen;
    bool visitado;
    arista *adyacente;
    vertice *sgteVertice;
};
typedef vertice *pvertice;


class grafo {
private:
    pvertice pGrafo;

public:
	int numVertices;
    int numAristas;
    grafo();
    ~grafo();

    void imprimirGrafo();
    void insertarVertice(int);
    bool existeVertice(int);
    void establecerVisita();
    void restablecerVisita();
    bool todosVisitados();
    void insertarVertice(pvertice);
    pvertice encontrarVertice(int v);
    void insertarAristaNoDirigida(parista);

    bool buscaCircuitoDFS(int visit[], int v, int padrev);

    bool existeCircuito();

    //Metodos de apoyo
    parista* aristasAdyacentes(int*, int n);
    int sumPesoCamino(int x, int y, int sum);
    void eliminarAristaNoDirigida(parista a);
    void eliminarVertice(int x);
    bool existeArista(int x, int y, int peso);
    void insertarAristaNoDirigida(int x, int y, int peso);
    void insertarArista(int x, int y, int peso);
    parista aristaMenorPesoCamino(int *vertices, grafo &T, int n);
    parista aristaMenorPesoCamino(int *vertices, grafo &T, int n, int cota);
    int *listaVertices();

    //Retorna un arbol con Los menores caminos
    grafo dijkstra(int v);
};

grafo::grafo() {
    pGrafo = NULL;
    numVertices=0;
    numAristas=0;
}

grafo::~grafo() {
    pvertice p, rp;
    parista r, ra;
    p = pGrafo;
    while (p != NULL) {
        r = p->adyacente;
        while (r != NULL) {
            ra = r;
            r = r->sgteArista;
            delete ra;
        }
        rp = p;
        p = p->sgteVertice;
        delete rp;
    }
}

void grafo::imprimirGrafo() {
    pvertice p;
    parista a;
    p = pGrafo;
    cout<<"------------------------------------------\n"; 
    cout << "Vert||Aristas" << endl;
    if (p == NULL)
        cout << "Grafo vacio" << endl;
    else{
        while (p != NULL) {
            cout << p->datoOrigen << ": ";
            a = p->adyacente;
            while (a != NULL) {
                cout << a->datoDestino<<" " ;
                a = a->sgteArista;
            }
            cout << endl;
            p = p->sgteVertice;
        }
    }
    cout<<"------------------------------------------\n"; 
}

void grafo::insertarVertice(int x) {
    if(existeVertice(x)) return;
	numVertices++;
    pvertice p;
    p = new vertice;
    p->datoOrigen = x;
    p->visitado = false;
    p->adyacente = NULL;
    p->sgteVertice = pGrafo;
    pGrafo = p;
}

bool grafo::existeVertice(int x){
    pvertice p;
    p = pGrafo;

    while(p != NULL){
        if(p->datoOrigen == x) return true;
        p=p->sgteVertice;
    }
    return false;
}

void grafo::establecerVisita(){
    pvertice p;
    p = pGrafo;

    while(p!=NULL){
        p->visitado = true;
        p = p->sgteVertice;
    }
}

void grafo::restablecerVisita(){
    pvertice p;
    p = pGrafo;

    while(p!=NULL){
        p->visitado = false;
        p = p->sgteVertice;
    }
}

bool grafo::todosVisitados(){
    pvertice p;
    p = pGrafo;

    while(p!=NULL){
        if(!(p->visitado)) return false;
        p = p->sgteVertice;
    }
    return true;
}


void grafo::insertarVertice(pvertice p){
    int x = p->datoOrigen;
    insertarVertice(x);
    
}

pvertice grafo::encontrarVertice(int v){
	pvertice p;
    p = pGrafo;
    
	while (p != NULL && p->datoOrigen != v)
    	p = p->sgteVertice;
            
    return p;
}


parista* grafo::aristasAdyacentes(int* vertices, int n){
	
	int j = 0;
	
	
	for(int i =0; i<n; i++){
		pvertice p = encontrarVertice(vertices[i]);
		parista a;

		a = p->adyacente;
		
        while (a != NULL) {
            if(pertenece(vertices, n, a->datoDestino)){
            	a = a->sgteArista;
            	continue;
			}
            j++;
            a = a->sgteArista;
        }		
	}
	
	parista* franja= new parista[j];	
	j=0;
	
	for(int i =0; i<n; i++){
		pvertice p = encontrarVertice(vertices[i]);
		parista a;

		a = p->adyacente;
        while (a != NULL) {
            if(pertenece(vertices, n, a->datoDestino)){
            	a = a->sgteArista;
            	continue;
			}
            franja[j++]=a;
            a = a->sgteArista;
        }		
	}	
	
	return franja;
}

int grafo::sumPesoCamino(int x, int y, int sum){
    pvertice p = encontrarVertice(x);
    parista a;
    a = p->adyacente;
    p->visitado = true;
    //printf("%d",x);
    while(a!=NULL){
        if(x == y){
            establecerVisita();
            return sum;
        }
        
        if(encontrarVertice(a->datoDestino)->visitado){
            a = a->sgteArista;
            continue;
        }
        sum = sumPesoCamino(a->datoDestino, y, sum + a->peso);
        if(!todosVisitados()) sum -= a->peso;
        //printf("%d\n",a->datoDestino);
        a = a->sgteArista;

    }
    return sum;
}

void grafo::eliminarAristaNoDirigida(parista a){
    pvertice p1, p2;
    parista a1, a2;
    parista b;

    p1 = encontrarVertice(a->datoDestino);
    p2 = encontrarVertice(a->datoOrigen);

    a1 = p1->adyacente;
    a2 = p2->adyacente;

    if(a1->datoDestino == p2->datoOrigen){
        p1->adyacente = a1->sgteArista;
    }
    else{
        b = a1;
        a1 = a1->sgteArista;
        while(a1->datoDestino != p2->datoOrigen && b!= NULL){
            b = a1;
            a1 = a1->sgteArista;
        } 
        b->sgteArista = a1->sgteArista;
    }

    if(a2->datoDestino == p1->datoOrigen){
        p2->adyacente = a2->sgteArista;
    }
    else{
        b = a2;
        a2 = a2->sgteArista;
        while(a2->datoDestino != p1->datoOrigen && b!= NULL){
            b = a2;
            a2 = a2->sgteArista;
        } 
        b->sgteArista = a2->sgteArista;
    }

    numAristas--;

}

void grafo::eliminarVertice(int x){
    pvertice v;
    v = pGrafo;

    if(v->datoOrigen == x){
        pGrafo = pGrafo->sgteVertice;
    }
    else{
        while((v->sgteVertice)->datoOrigen != x)
            v = v->sgteVertice;
        
        v->sgteVertice = (v->sgteVertice)->sgteVertice;
    }
    numVertices--;
}

bool grafo::existeArista(int x, int y, int peso){
    pvertice p;
    parista a;
    p = pGrafo;

    if (p == NULL)
        return false;
    else
        while (p != NULL) {
            a = p->adyacente;
            while (a != NULL) {
                if(a->datoOrigen == x && a->datoDestino == y && peso == a->peso) return true;
                a = a->sgteArista;
            }
            p = p->sgteVertice;
        }

    return false;
}


void grafo::insertarAristaNoDirigida(parista a){
    if(existeArista(a->datoOrigen, a->datoDestino, a->peso)) return;
    insertarAristaNoDirigida(a->datoOrigen, a->datoDestino, a->peso);
}

bool grafo::buscaCircuitoDFS(int visit[], int v, int padrev){
    visit[v]=v;

    pvertice p = encontrarVertice(v);
    parista a;

    a = p->adyacente;
    while(a != NULL){
        if(visit[a->datoDestino] == -1){
            if(buscaCircuitoDFS(visit, a->datoDestino, v)){
                return true;
            }
        }
        else if(padrev != a->datoDestino) return true;

        a = a->sgteArista;
    }

    return false;
}

bool grafo::existeCircuito(){

    //int visit[8];
    int *visit = new int[numVertices];

    for(int i=0; i<numVertices; i++) visit[i]=-1;

    for(int i=0; i<numVertices; i++)//{
        if(visit[i]==-1){
            if(buscaCircuitoDFS(visit, i, i))//{
                return true;
           // }
        //}
    }
    return false;
}


void grafo::insertarAristaNoDirigida(int x, int y, int peso) {
    if(existeArista(x, y, peso)) return;
    numAristas++;
	insertarArista(x, y, peso);
	insertarArista(y, x, peso);
}

void grafo::insertarArista(int x, int y, int peso) {
    pvertice p;
    parista a;
    p = pGrafo;
    if (p != NULL) {
        while (p != NULL && p->datoOrigen != x)
            p = p->sgteVertice;
        if (p != NULL) {
            a = new arista;
            a->datoOrigen = x;
            a->datoDestino = y;
            a->peso = peso; // Le asignamos el peso a la arista
            a->sgteArista = p->adyacente;
            p->adyacente = a;
        }
    }
}

parista grafo::aristaMenorPesoCamino(int* vertices,grafo &T, int n){

	return aristaMenorPesoCamino(vertices, T, n, INT_MAX);

}


parista grafo::aristaMenorPesoCamino(int* vertices,grafo &T, int n, int cota){
	//int j = 0;
	int min =cota;
    int neoMin;
	parista aMin = NULL;
	parista a;
    int datos[3];

	for(int i =0; i<n; i++){
		pvertice p = encontrarVertice(vertices[i]);
	
		a = p->adyacente;
        while (a != NULL) {
            if(pertenece(vertices, n, a->datoDestino)){
            	a = a->sgteArista;
            	continue;
			}

            T.insertarVertice(a->datoDestino);
            T.insertarAristaNoDirigida(a);
            neoMin = T.sumPesoCamino(0, a->datoDestino, 0);
            T.restablecerVisita();

            if(neoMin < min){
                min = neoMin;
                datos[0]=a->datoDestino;
                datos[1]= a->datoOrigen;
                datos[2]= a->peso;
            }

            T.eliminarAristaNoDirigida(a);
            T.eliminarVertice(a->datoDestino);

            
            a = a->sgteArista;
        }	
	}

    parista t;
    t = new arista;
    t->datoDestino = datos[0];
    t->datoOrigen = datos[1];
    t->peso = datos[2];

    return t;

}

int* grafo::listaVertices(){
	pvertice p = pGrafo;
	
	int * vertices = new int[numVertices];
	int i=0;
	while(p != NULL){
		vertices[i++]=p->datoOrigen;
		p=p->sgteVertice;
	}
	
	return vertices;
}


/*******************************
 *****Algoritmo Dijkstra********
 ******************************/
grafo grafo::dijkstra(int v){
    grafo T;
    parista* franja;
    parista aMin;

    T.insertarVertice(v);

    do{
        if(T.numVertices == numVertices) break;

	    int* vertices = T.listaVertices(); 
	    franja= aristasAdyacentes(vertices, T.numVertices);

	    parista aMin = aristaMenorPesoCamino(vertices, T, T.numVertices);
        
        T.imprimirGrafo();
        //imprimirArista(aMin);

	    v = aMin->datoDestino;
	    T.insertarVertice(v);
	    T.insertarAristaNoDirigida(aMin->datoOrigen, aMin->datoDestino, aMin->peso);
	}
	while(true);

    return T;   
}

int main() {
    grafo g;
    
    g.insertarVertice(0);
    g.insertarVertice(1);
    g.insertarVertice(2);
    g.insertarVertice(3);
    g.insertarVertice(4);
    g.insertarVertice(5);
    g.insertarVertice(6);
    g.insertarVertice(7);
    
    g.insertarAristaNoDirigida(0, 6, 53);
    g.insertarAristaNoDirigida(0, 1, 32);
	g.insertarAristaNoDirigida(0, 2, 29);
	g.insertarAristaNoDirigida(4,3,34);
	g.insertarAristaNoDirigida(5,3,18);
	g.insertarAristaNoDirigida(7,4,46);
	g.insertarAristaNoDirigida(5,4,40);
	g.insertarAristaNoDirigida(0,5,60);
	g.insertarAristaNoDirigida(6,4,51);
	g.insertarAristaNoDirigida(7,0,31);
	g.insertarAristaNoDirigida(7,6,25);
	g.insertarAristaNoDirigida(7,1,21);
	
    
    
    g.imprimirGrafo();   
	cout<<g.existeCircuito(); 

    grafo T;
    T = g.dijkstra(0);

    
    T.imprimirGrafo(); 
    cout<<T.existeCircuito()<<endl;
    
    return 0;

}
