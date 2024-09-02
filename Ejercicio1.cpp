#include <iostream>
#include <queue>
#include <stdint.h>
using namespace std;

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

};