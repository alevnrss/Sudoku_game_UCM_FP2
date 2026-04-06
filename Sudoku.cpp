#include "Sudoku.h"
#include <iostream>

void inicializaSudoku(tSudoku& s, int d) {
	inicializaTablero(s.tablero, d);		
	s.cont_numeros = 0;
	s.celdas_bloqueadas.cont = 0;
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			for(int k = 0; k < d; k++) {
				s.valores_celda.valores[i][j][k].posible = true;
				s.valores_celda.valores[i][j][k].celdas_que_afectan = 0;
			}
		}
	}
}

//funciones refactorizar cargar sudoku
void actualiza_valor_celda(tSudoku& s, int f, int c, int v) { // actualiza una celda concreta para el valor v
	int idx = v - 1;
	if (s.valores_celda.valores[f][c][idx].posible) {
		s.valores_celda.valores[f][c][idx].posible = false;
		s.valores_celda.valores[f][c][idx].celdas_que_afectan = 1;
	}
	else {
		s.valores_celda.valores[f][c][idx].celdas_que_afectan++;
	}
}
void annade_celdas_afectadas(tSudoku& s, int f, int c, int v) {
	//filas
	for (int col = 0; col < s.tablero.dimension; col++) {
		if (col != c)
			actualiza_valor_celda(s, f, col, v);
	}
	// Columnas
	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		if (fila != f)
			actualiza_valor_celda(s, fila, c, v);
	}
	// Subcuadricula
	int raiz = sqrt(s.tablero.dimension);
	int fi = f - (f % raiz);
	int ci = c - (c % raiz);
	for (int ff = fi; ff < fi + raiz; ff++) {
		for (int cc = ci; cc < ci + raiz; cc++) {
			if (ff != f || cc != c)
				actualiza_valor_celda(s, ff, cc, v);
		}
	}
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
				inicializaCelda(celda);	
			}
			else {
				inicializaCelda(celda, valor, ORIGINAL);	
				s.cont_numeros++;
				annade_celdas_afectadas(s, fila, columna, valor); 
			}

			pon_elemento(s.tablero, fila, columna, celda);	
		}
	}
	exito = true;
	return exito;
}

//refactorizacion funciones


void restaura_valor_celda(tSudoku& s, int f, int c, int v) { // restaura una celda concreta para el valor v
	int idx = v - 1; 
		s.valores_celda.valores[f][c][idx].celdas_que_afectan--;
	if(s.valores_celda.valores[f][c][idx].celdas_que_afectan==0) {
		s.valores_celda.valores[f][c][idx].posible = true;
	}
}
void elimina_celdas_afectadas(tSudoku& s, int f, int c, int v) {
	// Filas
	for (int col = 0; col < s.tablero.dimension; col++) {
		if (col != c)
			restaura_valor_celda(s, f, col, v);
	}
	// Columnas
	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		if (fila != f)
			restaura_valor_celda(s, fila, c, v);
	}
	// Subcuadricula
	int raiz = sqrt(s.tablero.dimension);
	int fi = f - (f % raiz);
	int ci = c - (c % raiz);
	for (int ff = fi; ff < fi + raiz; ff++) {
		for (int cc = ci; cc < ci + raiz; cc++) {
			if (ff != f || cc != c)
				restaura_valor_celda(s, ff, cc, v);
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

bool quitar_valor_sudoku(tSudoku& s, int f, int c) {
	bool e = false;
	int v;
	if (es_vacia(s.tablero.matriz[f][c]) == false && es_original(s.tablero.matriz[f][c]) == false) {
		v = dame_valor(s.tablero.matriz[f][c]);  // guardo el valor antes de borrarlo para poder usar en eliminar las celdas afectadas
		pon_vacia(s.tablero.matriz[f][c]);
		s.cont_numeros--;
		elimina_celdas_afectadas(s, f, c, v);
		quita_celdas_bloqueadas(s);
		e = true;
	}
	return e;
}
//fin funciones de refactorizacion
int dame_dimension(const tSudoku& s) {
	int dimension = s.tablero.dimension;
	return dimension;
}

tCelda dame_celda(const tSudoku& s, int f, int c) {
	return s.tablero.matriz[f][c];
}

bool esValida(tPosicion pos, const tSudoku& s) {
	bool e;
	if ((pos.fila >= s.tablero.dimension || pos.fila < 0) || (pos.columna >= s.tablero.dimension || pos.columna < 0) || (es_vacia(s.tablero.matriz[pos.fila][pos.columna]) == false)) {
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
		e = s.valores_celda.valores[pos.fila][pos.columna][v - 1].posible;

	}
	return e;
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

bool pon_valor_sudoku(tSudoku& s, int f, int c, int v) {
	bool e = false;
	if (es_valor_posible(s, {f,c}, v) == true) {
		pon_valor(s.tablero.matriz[f][c], v);	
		pon_ocupada(s.tablero.matriz[f][c]);	
		s.cont_numeros++;
		annade_celdas_afectadas(s, f, c, v);
		incorpora_celdas_bloqueadas(s, f, c);
		e = true;
	}
	return e;
}


// PASO 5: reset, autocompleta y terminado
void reset(tSudoku& s) {
	int contadorCeldasOriginales = 0;
	int v;
	for (int fila = 0; fila < s.tablero.dimension; fila++) {
		for (int columna = 0; columna < s.tablero.dimension; columna++) {
			if (s.tablero.matriz[fila][columna].estado == OCUPADA) {
				v = dame_valor(s.tablero.matriz[fila][columna]);
				inicializaCelda(s.tablero.matriz[fila][columna]);
				elimina_celdas_afectadas(s, fila, columna, v);
			}
			else if(s.tablero.matriz[fila][columna].estado==ORIGINAL){
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
			
			//comprobamos si esta VACIA
			if (es_vacia(s.tablero.matriz[fila][columna])) {
				int contadorPosibles = 0;
				int valorDetectado = 0;

				// probamos valores si esta VACIA (1-9)
				for (int v = 0; v < s.tablero.dimension; v++) {
					if (s.valores_celda.valores[fila][columna][v].posible) {
						contadorPosibles++;
						valorDetectado = v+1;
					}
				}

				// comprobar que si hay un solo posible rellenar de inmediato
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




