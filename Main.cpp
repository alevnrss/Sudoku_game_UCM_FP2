#include "Celda.h"
#include "Tablero.h"
#include "Sudoku.h"
#include "InputOutput.h"

#include <iostream>
#include <fstream>
using namespace std;

// constantes privadas
const string nombre_arch1 = "sudoku_1.txt";
const string nombre_arch2 = "sudoku_2.txt";

// Subprogramas privados de main
void abrirArchivo(bool& exito, ifstream& archivo);
int menu();


int main() {
	bool exito = false;
	ifstream archivo;
	tSudoku s;

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
			}
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



