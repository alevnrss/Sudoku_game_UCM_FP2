#include "Sudoku.h"
#include <iostream>

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

bool esValida(tPosicion pos, const tSudoku& s) {
	bool e;
	if ((pos.fila > s.tablero.dimension || pos.fila < 0) || (pos.columna > s.tablero.dimension || pos.columna < 0) || (es_vacia(s.tablero.matriz[pos.fila][pos.columna]) == false)) {
		e = false;
	}
	else {
		e = true;
	}

	return e;
}

bool busqueda_valor_filacolumna(const tSudoku& s, tPosicion pos, int v) {
	bool e = false;
	int cursor_fila = 0;
	int cursor_col = 0;
	while (!e && cursor_fila < s.tablero.dimension) {
		if (s.tablero.matriz[pos.fila][cursor_fila].valor == v) {
			e = true;
		}
		else {
			cursor_fila++;
		}
	}

	if (e == false) {
		while (!e && cursor_col < s.tablero.dimension) {
			if (s.tablero.matriz[cursor_col][pos.columna].valor == v) {
				e = true;
			}
			else {
				cursor_col++;
			}
		}
	}
	return e;
}

bool buscar_subcuadricula(const tSudoku& s, tPosicion pos, int v) {
	bool e = false;
	tPosicion pos_esquina_izq;
	int raizDim = sqrt(s.tablero.dimension);// tamaño de las subcuadriculas
	pos_esquina_izq.fila = pos.fila - (pos.fila % raizDim);
	pos_esquina_izq.columna = pos.columna - (pos.columna % raizDim);
	int cursor_fila = pos_esquina_izq.fila;
	int cursor_col = pos_esquina_izq.columna;

	while (!e && cursor_fila < pos_esquina_izq.fila + raizDim) {
		cursor_col = pos_esquina_izq.columna;
		while (!e && cursor_col < pos_esquina_izq.columna + raizDim) {
			if (s.tablero.matriz[cursor_fila][cursor_col].valor == v) {
				e = true;
			}
			else {
				cursor_col++;
			}
		}
		cursor_fila++;
	}

	return e;
}


bool es_valor_posible(const tSudoku& s, tPosicion pos, int v) {
	bool e;

	if(v < 1 || v > s.tablero.dimension) {
		e = false;
	}
	else {
		if (busqueda_valor_filacolumna(s, pos, v) == true || buscar_subcuadricula(s, pos, v) == true || esValida(pos, s) == false) {
			e = false;
		}
		else {
			e = true;
		}

		return e;
	}
	
}
void valores_posibles(const tSudoku& s, tPosicion pos) {
	cout << "Los valores posibles son: ";
	for (int i = 1; i < 10; i++) {
		if (es_valor_posible(s, pos, i) == true) {
			cout << i << " ";
		}
	}
	cout << "\n";
}

//paso 6 Subprogranas privados para comprobar bloqueos
bool esta_en_bloqueadas(const tSudoku& s, tPosicion pos) {
	bool encontrada = false;
	int i = 0;
	while (!encontrada && i < s.celdas_bloqueadas.cont) {
		if (s.celdas_bloqueadas.bloqueadas[i].fila == pos.fila &&
			s.celdas_bloqueadas.bloqueadas[i].columna == pos.columna) {
			encontrada = true;
		}
		else {
			i++;
		}
	}
	return encontrada;
}

//si la celda es vacia, sin valores posibles, y no esta ya bloqueada entonces la añade
void comprueba_y_bloquea(tSudoku& s, tPosicion pos) {
	if (es_vacia(s.tablero.matriz[pos.fila][pos.columna]) && !esta_en_bloqueadas(s, pos)) {
		bool tiene_posible = false;
		int v = 1;
		while (!tiene_posible && v <= s.tablero.dimension) {
			if (es_valor_posible(s, pos, v)) {
				tiene_posible = true;
			}
			else {
				v++;
			}
		}
		if (!tiene_posible) {
			s.celdas_bloqueadas.bloqueadas[s.celdas_bloqueadas.cont] = pos;
			s.celdas_bloqueadas.cont++;
		}
	}
}

void incorpora_celdas_bloqueadas(tSudoku& s, int f, int c) {
	tPosicion pos;

	// Recorre la fila f
	for (int col = 0; col < s.tablero.dimension; col++) {
		pos = { f, col };
		comprueba_y_bloquea(s, pos);
	}

	// Recorre la columna c
	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		pos = { fila, c };
		comprueba_y_bloquea(s, pos);
	}

	// Recorre la subcuadricula a la que pertenece (f,c)
	int raiz = sqrt(s.tablero.dimension);
	int fila_inicio = f - (f % raiz);
	int col_inicio = c - (c % raiz);
	for (int fi = fila_inicio; fi < fila_inicio + raiz; fi++) {
		for (int ci = col_inicio; ci < col_inicio + raiz; ci++) {
			pos = { fi, ci };
			comprueba_y_bloquea(s, pos);
		}
	}

}
// PASO 7 - privado
void quita_celdas_bloqueadas(tSudoku& s) {
	int i = 0;
	while (i < s.celdas_bloqueadas.cont) {
		tPosicion pos = s.celdas_bloqueadas.bloqueadas[i];

		bool tiene_posible = false;
		int v = 1;
		while (!tiene_posible && v <= s.tablero.dimension) {
			if (es_valor_posible(s, pos, v)) {
				tiene_posible = true;
			}
			else {
				v++;
			}
		}

		if (tiene_posible) {
			for (int j = i; j < s.celdas_bloqueadas.cont - 1; j++) {
				s.celdas_bloqueadas.bloqueadas[j] = s.celdas_bloqueadas.bloqueadas[j + 1];
			}
			s.celdas_bloqueadas.cont--;
		}
		else {
			i++;
		}
	}
}

bool pon_valor_sudoku(tSudoku& s, int f, int c, int v) {
	bool e = false;
	if (es_valor_posible(s, {f,c}, v) == true) {
		pon_valor(s.tablero.matriz[f][c], v);	// Subprograma de Celda.cpp
		pon_ocupada(s.tablero.matriz[f][c]);	// Subprograma de Celda.cpp
		s.cont_numeros++;
		incorpora_celdas_bloqueadas(s, f, c);
		e = true;
	}
	return e;
}

bool quitar_valor_sudoku(tSudoku& s, int f, int c) {
	bool e = false;
	if (es_vacia(s.tablero.matriz[f][c]) == false && es_original(s.tablero.matriz[f][c]) == false) {
		pon_vacia(s.tablero.matriz[f][c]);	// Subprograma de Celda.cpp
		s.cont_numeros--;
		quita_celdas_bloqueadas(s);
		e = true;
	}
	return e;
}

// PASO 5: reset, autocompleta y terminado
void reset(tSudoku& s) {
	int contadorCeldasOriginales = 0;

	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		for (int columna = 0; columna < s.tablero.dimension; columna++) {
			if (s.tablero.matriz[fila][columna].estado != ORIGINAL) {
				inicializaCelda(s.tablero.matriz[fila][columna]);
			}
			else {
				contadorCeldasOriginales++;
			}
		}
	}
	s.cont_numeros = contadorCeldasOriginales;
	s.celdas_bloqueadas.cont = 0;
}

void autocompleta(tSudoku& s) {
	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		for (int columna = 0; columna < s.tablero.dimension; columna++) {
			
			// Subtarea 1: Comprobamos si esta VACIA
			if (es_vacia(s.tablero.matriz[fila][columna])) {
				int contadorPosibles = 0;
				int valorDetectado = 0;

				// Subtarea 2: Probamos valores si esta VACIA (1-9)
				for (int v = 1; v <= 9; v++) {
					if (es_valor_posible(s, {fila, columna}, v)) {
						contadorPosibles++;
						valorDetectado = v;
					}
				}

				// Subtarea 3: Importante comprobar que si hay un solo posible rellenar de inmediato
				if (contadorPosibles == 1) {
					pon_valor_sudoku(s, fila, columna, valorDetectado);
				}
			}

		}
	}


}

bool terminado(const tSudoku& s) {
	// La dimension al ser 9, 9x9 es 81. Si el contador llega a ser 81 entonces se acabo el sudoku
	return s.cont_numeros == (s.tablero.dimension * s.tablero.dimension);
}

bool bloqueo(const tSudoku& s) {
	return s.celdas_bloqueadas.cont > 0;
}

int dame_num_celdas_bloqueadas(const tSudoku& s) {
	return s.celdas_bloqueadas.cont;
}

void dame_celda_bloqueada(const tSudoku& s, int p, int& f, int& c) {
	f = s.celdas_bloqueadas.bloqueadas[p].fila;
	c = s.celdas_bloqueadas.bloqueadas[p].columna;
}




