#include "Celda.h"
#include "Tablero.h"
#include "Sudoku.h"
#include "InputOutput.h"

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// constantes privadas
const string nombre_arch1 = "sudoku_1.txt";
const string nombre_arch2 = "sudoku_2.txt";

// Subprogramas privados de main
void abrirArchivo(bool& exito, ifstream& archivo);
int menu();
tPosicion pide_pos();
bool esValida(tPosicion pos, const tSudoku& s);
bool busqueda_valor_filacolumna(const tSudoku& s, tPosicion pos, int v);
bool buscar_subcuadricula(const tSudoku& s, tPosicion pos, int v);
void valores_posibles(const tSudoku& s, tPosicion pos);
int main() {
	bool exito = false;
	ifstream archivo;
	tSudoku s;
	tPosicion pos_introducido; //pos puesto por el usuario paso 3

	abrirArchivo(exito, archivo);
	if (!exito) {
		cout << "No se ha logrado abrir el archivo correctamente" << endl;
	}
	else {
		int opcion = 0;
		cout << "Archivo abierto correctamente" << endl;
		carga_sudoku(archivo, s);
		mostrar_juego_consola(s);
		opcion = menu();
		while (opcion != 6) {
			switch (opcion) {
			case 1: 
				cout << "Usted ha elegido la opcion 1.-" << endl;
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				pos_introducido = pide_pos();
				if (esValida(pos_introducido, s) == false) {
					cout << "Posicion introducida no valida" << endl;
				}
				else {
					valores_posibles(s, pos_introducido);
				}
				break;
			}
			mostrar_juego_consola(s);
			opcion = menu();
		}
		cout << "Usted ha elegido la opcion 6.- salir." << endl;
	}

	return 0;
}

int menu() {
	int opcion = -1; 
	while (opcion < 1 || opcion > 6) {
		cout << "1.- poner valor" << endl;
		cout << "2.- quitar valor" << endl;
		cout << "3.- reset valor" << endl;
		cout << "4.- posibles valores de una celda vacia" << endl;
		cout << "5.- autocompletar celdas con valor unico" << endl;
		cout << "6.- salir" << endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		if (opcion < 1 || opcion > 6) {
			cout << "Opcion NO valida." << endl;
		}
	}
	
	return opcion; 
}

void abrirArchivo(bool& exito, ifstream& archivo) {
	archivo.open(nombre_arch1);
	if (!archivo.is_open()) {
		exito = false;
	}
	else {
		exito = true;
	}
}	

tPosicion pide_pos() {
	tPosicion pos;
	cout << "Introduce la fila: "<<endl;
	cin >> pos.fila;
	cout << "Introduce la columna: " << endl;
	cin >> pos.columna;

	pos.fila = pos.fila -1 ;
	pos.columna = pos.columna -1;
	return pos;
}

bool esValida(tPosicion pos, const tSudoku& s){
	bool e;
	if ((pos.fila > s.tablero.dimension || pos.fila < -1) || (pos.columna > s.tablero.dimension || pos.columna < -1)|| (es_vacia(s.tablero.matriz[pos.fila][pos.columna]) == false)){
		e = false;
	}else {
		e = true;
	}

	return e;
}

bool es_valor_posible(const tSudoku& s, tPosicion pos, int v) {
	bool e;
	if (busqueda_valor_filacolumna(s, pos, v) == true || buscar_subcuadricula(s, pos, v) == true|| esValida(pos,s)==false) {
		e = false;
	}
	else {
		e = true;
	}

	return e;
}

bool busqueda_valor_filacolumna(const tSudoku& s,tPosicion pos, int v) {
	bool e=false;
	int cursor_fila = 0;
	int cursor_col = 0;
	while (!e && cursor_fila < s.tablero.dimension){
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
	bool e=false;
	tPosicion pos_esquina_izq;
	int raizDim = sqrt(s.tablero.dimension);// tamaño de las subcuadriculas
	pos_esquina_izq.fila = pos.fila - (pos.fila % raizDim);
	pos_esquina_izq.columna = pos.columna - (pos.columna % raizDim);
	int cursor_fila = pos_esquina_izq.fila;
	int cursor_col = pos_esquina_izq.columna;

	while (!e && cursor_fila < pos_esquina_izq.fila + raizDim) {
		cursor_col = pos_esquina_izq.columna;
		while (!e && cursor_col < pos_esquina_izq.columna+ raizDim) {
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

void valores_posibles(const tSudoku& s,tPosicion pos) {
	cout << "Los valores posibles son: ";
	for (int i = 1; i < 10; i++) {
		if (es_valor_posible(s, pos, i) == true) {
			cout << i << " ";
		}
	}
	cout << "\n";
}