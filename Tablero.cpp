//NOMBRES Y APELLIDOS: Leiton Campillos y Alejandro Veneros
//GRUPO:7
#include "Tablero.h"
#include "Celda.h"

void inicializaTablero(tTablero& t, int dimension) {
	t.dimension = dimension;
	for (int f = 0; f < dimension; f++) {
		for (int c = 0; c < dimension; c++) {
			inicializaCelda(t.matriz[f][c]);
		}
	}
}

int dame_dimension(const tTablero& t) {
	return t.dimension;
}
const tCelda& dame_elem(const tTablero& t, int f, int c) {
	return t.matriz[f][c];
}

void pon_elemento(tTablero& t, int f, int c, tCelda celda) {
	if (f >= 0 && f < t.dimension && c >= 0 && c < t.dimension) {
		t.matriz[f][c] = celda;
	}
}