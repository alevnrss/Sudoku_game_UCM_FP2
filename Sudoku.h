//NOMBRES Y APELLIDOS: Leiton Campillos y Alejandro Veneros
//GRUPO:7
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
typedef int arrValores[DIM];

struct tPosicion {
	int fila, columna;
};

struct tBloqueos {
	int cont;
	tPosicion bloqueadas[DIM * DIM];
};

struct tValor {
	bool posible;
	int celdas_que_afectan;
};
struct tValores {
	int dimension;
	tValor valores[DIM][DIM][DIM];
};

struct tSudoku {
	tTablero tablero;
	int cont_numeros;// contador de celdas no vacias
	tBloqueos celdas_bloqueadas;
	tValores valores_celda;
};


// SUBPROGRAMAS PUBLICOS

// Operaciones de inicializacion 
void inicializaSudoku(tSudoku& s, int d);				// PASO 2
bool carga_sudoku(ifstream& archivo, tSudoku& s);		// PASO 2

// Operaciones de consulta del estado
int dame_dimension(const tSudoku& s);
tCelda dame_celda(const tSudoku& s, int fila, int columna);
bool terminado(const tSudoku& s);
bool esValida(tPosicion pos, const tSudoku& s);
bool es_valor_posible(const tSudoku& s, tPosicion pos, int v);
void valores_posibles(const tSudoku& s, tPosicion pos);

// version 2
int dame_num_celdas_libres(const tSudoku& s);
void numero_posibles_valores(const tSudoku& s, arrValores av);

// Operaciones de modificacion del estado
bool pon_valor_sudoku(tSudoku& s, int f, int c, int v);
bool quitar_valor_sudoku(tSudoku& s, int f, int c);
void reset(tSudoku& s);
void autocompleta(tSudoku& s);

//Consulta bloqueos
bool bloqueo(const tSudoku& s);
int dame_num_celdas_bloqueadas(const tSudoku& s);
void dame_celda_bloqueada(const tSudoku& s, int p, int& f, int& c);

// version 2
// sobrecargar de operaciones relacionales
bool operator==(const tSudoku& s1, const tSudoku& s2);
bool operator<(const tSudoku& s1, const tSudoku& s2);

#endif
