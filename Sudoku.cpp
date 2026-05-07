//NOMBRES Y APELLIDOS: Leiton Campillos y Alejandro Veneros
//GRUPO:7
#include "Sudoku.h"
#include <iostream>
#include <cmath>

void inicializaSudoku(tSudoku& s, int d) {
	inicializaTablero(s.tablero, d);
	s.cont_numeros = 0;
	s.celdas_bloqueadas.cont = 0;
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			for (int k = 0; k < d; k++) {
				s.valores_celda.valores[i][j][k].posible = true;
				s.valores_celda.valores[i][j][k].celdas_que_afectan = 0;
			}
		}
	}
}

// Subprogramas privados

void actualiza_valor_celda(tSudoku& s, int f, int c, int v) {
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
	int dim = dame_dimension(s);
	int raiz = sqrt(dim);
	// Filas
	for (int col = 0; col < dim; col++) {
		if (col != c)
			actualiza_valor_celda(s, f, col, v);
	}
	// Columnas
	for (int fila = 0; fila < dim; fila++) {
		if (fila != f)
			actualiza_valor_celda(s, fila, c, v);
	}
	// Subcuadricula
	int fi = f - (f % raiz);
	int ci = c - (c % raiz);
	for (int ff = fi; ff < fi + raiz; ff++) {
		for (int cc = ci; cc < ci + raiz; cc++) {
			if (ff != f || cc != c)
				actualiza_valor_celda(s, ff, cc, v);
		}
	}
}

void restaura_valor_celda(tSudoku& s, int f, int c, int v) {
	int idx = v - 1;
	s.valores_celda.valores[f][c][idx].celdas_que_afectan--;
	if (s.valores_celda.valores[f][c][idx].celdas_que_afectan == 0) {
		s.valores_celda.valores[f][c][idx].posible = true;
	}
}

void elimina_celdas_afectadas(tSudoku& s, int f, int c, int v) {
	int dim = dame_dimension(s);
	int raiz = sqrt(dim);
	// Filas
	for (int col = 0; col < dim; col++) {
		if (col != c)
			restaura_valor_celda(s, f, col, v);
	}
	// Columnas
	for (int fila = 0; fila < dim; fila++) {
		if (fila != f)
			restaura_valor_celda(s, fila, c, v);
	}
	// Subcuadricula
	int fi = f - (f % raiz);
	int ci = c - (c % raiz);
	for (int ff = fi; ff < fi + raiz; ff++) {
		for (int cc = ci; cc < ci + raiz; cc++) {
			if (ff != f || cc != c)
				restaura_valor_celda(s, ff, cc, v);
		}
	}
}

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

void comprueba_y_bloquea(tSudoku& s, tPosicion pos) {
	tCelda celda = dame_celda(s, pos.fila, pos.columna);
	if (es_vacia(celda) && !esta_en_bloqueadas(s, pos)) {
		bool tiene_posible = false;
		int v = 1;
		while (!tiene_posible && v <= dame_dimension(s)) {
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
	int dim = dame_dimension(s);
	int raiz = sqrt(dim);
	tPosicion pos;
	// Fila f
	for (int col = 0; col < dim; col++) {
		pos = { f, col };
		comprueba_y_bloquea(s, pos);
	}
	// Columna c
	for (int fila = 0; fila < dim; fila++) {
		pos = { fila, c };
		comprueba_y_bloquea(s, pos);
	}
	// Subcuadricula
	int fila_inicio = f - (f % raiz);
	int col_inicio = c - (c % raiz);
	for (int fi = fila_inicio; fi < fila_inicio + raiz; fi++) {
		for (int ci = col_inicio; ci < col_inicio + raiz; ci++) {
			pos = { fi, ci };
			comprueba_y_bloquea(s, pos);
		}
	}
}

void quita_celdas_bloqueadas(tSudoku& s) {
	int i = 0;
	while (i < s.celdas_bloqueadas.cont) {
		tPosicion pos = s.celdas_bloqueadas.bloqueadas[i];
		bool tiene_posible = false;
		int v = 1;
		while (!tiene_posible && v <= dame_dimension(s)) {
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

// Subprogramas publicos

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

int dame_dimension(const tSudoku& s) {
	return dame_dimension(s.tablero);
}

tCelda dame_celda(const tSudoku& s, int f, int c) {
	return dame_elem(s.tablero, f, c);
}

bool esValida(tPosicion pos, const tSudoku& s) {
	bool e;
	int dim = dame_dimension(s);
	if (pos.fila < 0 || pos.fila >= dim || pos.columna < 0 || pos.columna >= dim ||
		!es_vacia(dame_celda(s, pos.fila, pos.columna))) {
		e = false;
	}
	else {
		e = true;
	}
	return e;
}

bool busqueda_valor_filacolumna(const tSudoku& s, tPosicion pos, int v) {
	bool e = false;
	int dim = dame_dimension(s);
	int cursor_fila = 0;
	int cursor_col = 0;
	while (!e && cursor_fila < dim) {
		if (dame_valor(dame_celda(s, pos.fila, cursor_fila)) == v) {
			e = true;
		}
		else {
			cursor_fila++;
		}
	}
	if (!e) {
		while (!e && cursor_col < dim) {
			if (dame_valor(dame_celda(s, cursor_col, pos.columna)) == v) {
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
	int raizDim = sqrt(dame_dimension(s));
	tPosicion pos_esquina_izq;
	pos_esquina_izq.fila = pos.fila - (pos.fila % raizDim);
	pos_esquina_izq.columna = pos.columna - (pos.columna % raizDim);
	int cursor_fila = pos_esquina_izq.fila;
	int cursor_col = pos_esquina_izq.columna;
	while (!e && cursor_fila < pos_esquina_izq.fila + raizDim) {
		cursor_col = pos_esquina_izq.columna;
		while (!e && cursor_col < pos_esquina_izq.columna + raizDim) {
			if (dame_valor(dame_celda(s, cursor_fila, cursor_col)) == v) {
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
	if (v < 1 || v > dame_dimension(s)) {
		e = false;
	}
	else {
		e = s.valores_celda.valores[pos.fila][pos.columna][v - 1].posible;
	}
	return e;
}

void valores_posibles(const tSudoku& s, tPosicion pos) {
	cout << "Los valores posibles son: ";
	for (int i = 1; i <= dame_dimension(s); i++) {
		if (es_valor_posible(s, pos, i)) {
			cout << i << " ";
		}
	}
	cout << "\n";
}

bool pon_valor_sudoku(tSudoku& s, int f, int c, int v) {
	bool e = false;
	if (es_valor_posible(s, { f, c }, v)) {
		tCelda celda = dame_celda(s, f, c);
		pon_valor(celda, v);
		pon_ocupada(celda);
		pon_elemento(s.tablero, f, c, celda);
		s.cont_numeros++;
		annade_celdas_afectadas(s, f, c, v);
		incorpora_celdas_bloqueadas(s, f, c);
		e = true;
	}
	return e;
}

bool quitar_valor_sudoku(tSudoku& s, int f, int c) {
	bool e = false;
	tCelda celda = dame_celda(s, f, c);
	if (!es_vacia(celda) && !es_original(celda)) {
		int v = dame_valor(celda);
		pon_vacia(celda);
		pon_elemento(s.tablero, f, c, celda);
		s.cont_numeros--;
		elimina_celdas_afectadas(s, f, c, v);
		quita_celdas_bloqueadas(s);
		e = true;
	}
	return e;
}

void reset(tSudoku& s) {
	int dim = dame_dimension(s);
	int contadorCeldasOriginales = 0;
	for (int fila = 0; fila < dim; fila++) {
		for (int columna = 0; columna < dim; columna++) {
			tCelda celda = dame_celda(s, fila, columna);
			if (es_ocupada(celda)) {
				int v = dame_valor(celda);
				inicializaCelda(celda);
				pon_elemento(s.tablero, fila, columna, celda);
				elimina_celdas_afectadas(s, fila, columna, v);
			}
			else if (es_original(celda)) {
				contadorCeldasOriginales++;
			}
		}
	}
	s.cont_numeros = contadorCeldasOriginales;
	s.celdas_bloqueadas.cont = 0;
}

void autocompleta(tSudoku& s) {
	int dim = dame_dimension(s);
	bool cambios;
	do {
		cambios = false;
		for (int fila = 0; fila < dim; fila++) {
			for (int columna = 0; columna < dim; columna++) {
				if (es_vacia(dame_celda(s, fila, columna))) {
					int contadorPosibles = 0;
					int valorDetectado = 0;
					for (int v = 0; v < dim; v++) {
						if (s.valores_celda.valores[fila][columna][v].posible) {
							contadorPosibles++;
							valorDetectado = v + 1;
						}
					}
					if (contadorPosibles == 1) {
						pon_valor_sudoku(s, fila, columna, valorDetectado);
						cambios = true;
					}
				}
			}
		}
	} while (cambios);
}

bool terminado(const tSudoku& s) {
	int dim = dame_dimension(s);
	return s.cont_numeros == (dim * dim);
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

int dame_num_celdas_libres(const tSudoku& s) {
	int dim = dame_dimension(s);
	int contador = 0;
	for (int f = 0; f < dim; f++) {
		for (int c = 0; c < dim; c++) {
			if (es_vacia(dame_celda(s, f, c))) {
				contador++;
			}
		}
	}
	return contador;
}

void numero_posibles_valores(const tSudoku& s, arrValores av) {
	int dim = dame_dimension(s);
	for (int i = 0; i < DIM; i++) {
		av[i] = 0;
	}
	for (int f = 0; f < dim; f++) {
		for (int c = 0; c < dim; c++) {
			if (es_vacia(dame_celda(s, f, c))) {
				int nPosibles = 0;
				for (int v = 1; v <= dim; v++) {
					if (es_valor_posible(s, { f, c }, v)) {
						nPosibles++;
					}
				}
				if (nPosibles > 0 && nPosibles <= DIM) {
					av[nPosibles - 1]++;
				}
			}
		}
	}
}

bool operator==(const tSudoku& s1, const tSudoku& s2) {
	bool iguales = false;
	if (dame_num_celdas_libres(s1) == dame_num_celdas_libres(s2)) {
		arrValores av1, av2;
		numero_posibles_valores(s1, av1);
		numero_posibles_valores(s2, av2);
		int i = 0;
		while (i < DIM && av1[i] == av2[i]) {
			i++;
		}
		iguales = (i == DIM);
	}
	return iguales;
}

bool operator<(const tSudoku& s1, const tSudoku& s2) {
	bool menor = false;
	int libres1 = dame_num_celdas_libres(s1);
	int libres2 = dame_num_celdas_libres(s2);
	if (libres1 < libres2) {
		menor = true;
	}
	else if (libres1 == libres2) {
		arrValores av1, av2;
		numero_posibles_valores(s1, av1);
		numero_posibles_valores(s2, av2);
		int i = 0;
		while (i < DIM && av1[i] == av2[i]) {
			i++;
		}
		if (i < DIM) {
			menor = av1[i] > av2[i];
		}
	}
	return menor;
}