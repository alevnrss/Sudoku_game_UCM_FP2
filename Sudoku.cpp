#include "Sudoku.h"

void inicializaSudoku(tSudoku& s, int d) {
	inicializaTablero(s.tablero, d);		// subprograma de Tablero.cpp
	s.cont_numeros = 0;
	s.celdas_bloqueadas.cont = 0;
}

bool carga_sudoku(ifstream& archivo, tSudoku& s) {
	int dimension, valor;
	bool exito = false;
	
	archivo >> dimension;
	inicializaSudoku(s, dimension);
	for (int fila = 0; fila < dimension; fila++) {
		for (int columna = 0; columna < dimension; columna++) {
			archivo >> valor;

			tCelda celda;

			if (valor == 0) {
				inicializaCelda(celda);	// subprograma de Celda.cpp
			}
			else {
				inicializaCelda(celda, valor, ORIGINAL);	// Subprograma de Celda.cpp
				s.cont_numeros++;
			}

			pon_elemento(s.tablero, fila, columna, celda);	// Subprograma de Tablero.cpp
		}
	}
	exito = true;
	return exito;
}

int dame_dimension(const tSudoku& s) {
	int dimension = s.tablero.dimension;
	return dimension;
}

tCelda dame_celda(const tSudoku& s, int f, int c) {
	return s.tablero.matriz[f][c];
}



