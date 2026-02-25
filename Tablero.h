#ifndef tablero_h
#define tablero_h

#include "Celda.h"		// Incluimos el header de celda porque sera necesario en tablero.h


// variables globales constantes
const int DIM = 16;

// Estructuras
struct tTablero {
	int dimension;
	tCelda matriz[DIM][DIM];			// Array bidimensional que guardara datos de tipo tCelda
};

// Operaciones de inicializacion 
void inicializaTablero(tTablero& t, int dimension); // Inicial el tableto t a una dimension d
	
// Operaciones de consulta del estado 
int dame_dimension(const tTablero& t);	// Devuelve la dimension real del tablero
const tCelda& dame_elem(const tTablero& t, int f, int c);	// Devuelve la celda del tablero ubicado en dicha fila y columna

// Operaciones de modificacion del estado 
void pon_elemento(tTablero& t, int f, int c, tCelda celda);	// Coloca la celda en la posicion (f,c)
#endif
