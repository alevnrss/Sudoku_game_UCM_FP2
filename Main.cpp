#include "Celda.h"
#include "Tablero.h"
#include <iostream>
using namespace std;

// prototipos
void dibujarTablero(const tTablero& t);
string toString(tEstado estado);

int main() {

	tTablero t;
	int dimension = 4;

	inicializaTablero(t, 4);
	dibujarTablero(t);

	return 0;
}

void dibujarTablero(const tTablero& t) {
	for (int i = 0; i < t.dimension; i++) {
		for (int j = 0; j < t.dimension; j++) {
			cout << i << " X " << j << endl;
			cout << "estado: " << toString(t.matriz[i][j].estado) << endl;
			cout << "valor: " <<t.matriz[i][j].valor << endl;
		}
	}
}

string toString(tEstado estado) {
	string cadena;
	switch (estado) {
	case VACIA:
		return cadena = "vacia";
		break;

	case OCUPADA:
		return cadena = "ocupada";
		break;

	case ORIGINAL:
		return cadena = "original";
		break;
	}
}


