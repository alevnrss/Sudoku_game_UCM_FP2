#ifndef listasudokus_h
#define listasudokus_h

#include "sudoku.h"

const int MAX_LISTA = 2;
struct tListaSudokus {
	int cont;
	int capacidad;
	tSudoku** sudokus;

};


// Subprogramas publicos
// OPERACION DE INICIALIZACION
void inicializa_lista_sudokus(tListaSudokus& lista);

// OPERANCION DE DESTRUCCION
void destruye_lista_sudokus(tListaSudokus& lista);

// OPERACIONES DE CONSULTA DEL ESTADO
int dame_num_elems(const tListaSudokus& lista);
tSudoku* dame_sudoku(const tListaSudokus& lista, int i);

// OPERACIONES DE MODIFICACION DEL ESTADO
void insertar(tListaSudokus& lista, tSudoku* s);
void eliminar(tListaSudokus& lista, int pos);

// OPERACIONES DE VISUALIZACION DEL ESTADO
void mostrar_lista(const tListaSudokus& lista);


#endif
