#ifndef Sudoku_h
#define Sudoku_h

#include "Tablero.h"
#include <fstream>
using namespace std;

/**
* MODULO SUDOKU: 
* Este modulo contendra lo siguiente:
*	- Tablero que almacena el sudoku -> tipo tTablero que guarda valor y dimension.
*	- Contador de celdas no vacias -> tipo entero.
*	- Lista de operaciones bloqueadas -> tipo tPosicion
*/


// estructuras
struct tPosicion {
	int fila, columna;
};

struct tBloqueos {
	int cont;
	tPosicion bloqueadas[DIM * DIM];
};

struct tSudoku {
	tTablero tablero;
	int cont_numeros;// contador de celdas no vacias
	tBloqueos celdas_bloqueadas;
};

// SUBPROGRAMAS PUBLICOS

// Operaciones de inicializacion 
void inicializaSudoku(tSudoku& s, int d);				// PASO 2
bool carga_sudoku(ifstream& archivo, tSudoku& s);		// PASO 2

// Operaciones de consulta del estado
int dame_dimension(const tSudoku& s);
tCelda dame_celda(const tSudoku& s, int fila, int columna);
bool esValida(tPosicion pos, const tSudoku& s);
bool es_valor_posible(const tSudoku& s, tPosicion pos, int v);

// Operaciones de modificacion del estado
bool pon_valor_sudoku(tSudoku& s, int f, int c, int v);
bool quitar_valor_sudoku(tSudoku& s, int f, int c);
#endif