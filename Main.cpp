#include "Celda.h"
#include "Tablero.h"
#include "Sudoku.h"
#include "InputOutput.h"
#include "ListaSudokus.h"
#include "checkML.h"
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
int pide_valor();
void valores_posibles(const tSudoku& s, tPosicion pos);

// Subprogramas version 2
bool cargar_lista_sudokus_nuevos(string nombre_fichero, tListaSudokus& l );
void ejecutar_opcion(int opcion, tSudoku& s);
bool cargar_lista_sudokus_comenzados(string nombre_fichero, tListaSudokus& ls);
void guardar_lista_sudokus_comenzados(string nombre_fichero, const tListaSudokus& ls);
bool jugar(tSudoku& s);

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//int* punt = new int(4);  --> comprobar fugas
	tListaSudokus lista;
	inicializa_lista_sudokus(lista);

	if (!cargar_lista_sudokus_comenzados("lista_comenzados.txt", lista)) {

		cargar_lista_sudokus_nuevos("lista_sudokus.txt", lista);
	}

	int opcionLista = -1;
	while (opcionLista != 0) {
		cout << "--- MENU PRINCIPAL: SUDOKU ---" << endl;
		mostrar_lista(lista);
		cout << "0.- Salir y GUARDAR todo" << endl;
		cout << "Elige un sudoku (1 a " << dame_num_elems(lista) << "): ";
		cin >> opcionLista;

		if (opcionLista > 0 && opcionLista <= dame_num_elems(lista)) {
			
			tSudoku* s = dame_sudoku(lista, opcionLista - 1);

			bool abandonado = jugar(*s);

			if (!abandonado) {
				cout << "ENHORABUENA, Lo has terminado." << endl;
				
			}
			else {
				cout << "Partida pausada. Se guardara tu progreso." << endl;
			}
		}
	}
	
	guardar_lista_sudokus_comenzados("lista_comenzados.txt", lista);
	destruye_lista_sudokus(lista);

	cout << "Hasta pronto" << endl;
	return 0;
}

int menu() {
	int opcion = -1; 
	while (opcion < 1 || opcion > 6) {
		cout << "1.- poner valor" << endl;
		cout << "2.- quitar valor" << endl;
		cout << "3.- reset" << endl;
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
	archivo.open(nombre_arch2);
	if (!archivo.is_open()) {
		exito = false;
	}
	else {
		exito = true;
	}
}	

tPosicion pide_pos() {
	tPosicion pos;
	pos.fila = 0;
	pos.columna = 0;
	while (pos.fila < 1 || pos.fila > 9) {
		cout << "Introduce la fila: " << endl;
		cin >> pos.fila;
		if(pos.fila < 1 || pos.fila > 9){
			cout<<"Fila no valida, introducela de nuevo"<<endl;
		}
	}
	while (pos.columna < 1 || pos.columna > 9) {
		cout << "Introduce la columna: " << endl;
		cin >> pos.columna;
		if(pos.columna < 1 || pos.columna > 9){
			cout<<"Columna no valida, introducela de nuevo"<<endl;
		}
	}
	pos.fila = pos.fila -1 ;
	pos.columna = pos.columna -1;
	return pos;
}

int pide_valor() {
	int valor;
	cout << "Introduce el valor: " << endl;
	cin >> valor;
	return valor;
}

void ejecutar_opcion(int opcion, tSudoku& s) {
	tPosicion pos;
	int valor;

	switch (opcion) {
	case 1: // Poner
		pos = pide_pos();
		valor = pide_valor();
		if (!pon_valor_sudoku(s, pos.fila, pos.columna, valor))
			cout << "Movimiento no permitido." << endl;
		break;
	case 2: // Quitar
		pos = pide_pos();
		if (!quitar_valor_sudoku(s, pos.fila, pos.columna))
			cout << "No se puede quitar ese valor." << endl;
		break;
	case 3: // Reset
		reset(s);
		break;
	case 4: // Sugerencia
		pos = pide_pos();
		valores_posibles(s, pos);
		break;
	case 5: // Autocompletar
		autocompleta(s);
		break;
	}
}

bool cargar_lista_sudokus_nuevos(string nombre_fichero, tListaSudokus& l) {
	ifstream archivo;
	archivo.open(nombre_fichero);
	bool exito = false;

	if (archivo.is_open()) {
		int numSudokus;
		archivo >> numSudokus;

		string nombreSudoku;
		for (int i = 0; i < numSudokus; i++) {
			archivo >> nombreSudoku;
			tSudoku* s = new tSudoku;
			ifstream fSudoku;
			fSudoku.open(nombreSudoku);

			if (fSudoku.is_open()) {
				carga_sudoku(fSudoku, *s);
				insertar(l, s);
				fSudoku.close();
			}
			else {
				delete s;
			}

		}

		exito = true;
		archivo.close();
	}

	return exito;
}

bool cargar_lista_sudokus_comenzados(string nombre_fichero, tListaSudokus& ls) {
	ifstream archivo;
	archivo.open(nombre_fichero);
	bool exito = false;

	if (archivo.is_open()) {
		int numSudokus;
		archivo >> numSudokus;
		for (int i = 0; i < numSudokus; i++) {
			tSudoku* s = new tSudoku;
			if (carga_sudoku(archivo, *s)) {
				int fila, col, valor;
				archivo >> fila;
				while (fila != -1) {
					archivo >> col >> valor;
					
					pon_valor_sudoku(*s, fila, col, valor);
					archivo >> fila; 
				}
				insertar(ls, s);
			}
			else {
				delete s;
			}
		}
		exito = true;
		archivo.close();
	}
	return exito;
}

void guardar_lista_sudokus_comenzados(string nombre_fichero, const tListaSudokus& ls) {
	ofstream archivo;
	archivo.open(nombre_fichero);

	if (archivo.is_open()) {
		archivo << ls.cont << endl; 

		for (int i = 0; i < ls.cont; i++) {
			tSudoku* s = ls.sudokus[i];
			archivo << s->tablero.dimension << endl;
			for (int f = 0; f < s->tablero.dimension; f++) {
				for (int c = 0; c < s->tablero.dimension; c++) {
					if (es_original(s->tablero.matriz[f][c]))
						archivo << dame_valor(s->tablero.matriz[f][c]) << " ";
					else
						archivo << "0 "; 
				}
				archivo << endl;
			}

			for (int f = 0; f < s->tablero.dimension; f++) {
				for (int c = 0; c < s->tablero.dimension; c++) {
					if (s->tablero.matriz[f][c].estado == OCUPADA) {
						archivo << f << " " << c << " " << dame_valor(s->tablero.matriz[f][c]) << endl;
					}
				}
			}
			archivo << "-1" << endl; 
		}
		archivo.close();
	}
}

bool jugar(tSudoku& s) {
	int opcion = 0;
	bool abandonado = false;

	while (opcion != 6 && !terminado(s)) {
		mostrar_juego_consola(s);

		if (bloqueo(s)) {
			if (bloqueo(s)) {
				cout << "Has cometido un error previo. El Sudoku no tiene solucion actual." << endl;
				cout << "Debes quitar algun valor para desbloquear las casillas: ";
			}
			for (int i = 0; i < dame_num_celdas_bloqueadas(s); i++) {
				int f, c;
				dame_celda_bloqueada(s, i, f, c);
				cout << "(" << f + 1 << "," << c + 1 << ") ";
			}
			cout << endl;
		}

		opcion = menu();
		if (opcion == 6) {
			abandonado = true;
		}
		else {
			ejecutar_opcion(opcion, s); // La función que hicimos antes
		}
	}

	if (terminado(s)) {
		mostrar_juego_consola(s);
		cout << "Sudoku completado con exito" << endl;
	}

	return abandonado;
}


