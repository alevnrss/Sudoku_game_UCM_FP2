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
int pide_valor();
void valores_posibles(const tSudoku& s, tPosicion pos);
int main() {
	bool exito = false;
	ifstream archivo;
	tSudoku s;
	tPosicion pos_introducido; //pos puesto por el usuario paso 3
	int valor_introducido; //valor puesto por el usuario paso 4

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
				pos_introducido = pide_pos();
				valor_introducido = pide_valor();
				if(pon_valor_sudoku(s,pos_introducido.fila,pos_introducido.columna,valor_introducido) == true){
					cout<<"Valor introducido correctamente"<<endl;
				}
				else{
					cout<<"Valor introducido no valido"<<endl;
				}

				break;
			case 2:
				pos_introducido = pide_pos();
				if(quitar_valor_sudoku(s,pos_introducido.fila,pos_introducido.columna) == true){
					cout<<"Valor quitado correctamente"<<endl;
				}
				else{
					cout<<"Valor introducido no valido"<<endl;
				}
				break;
			case 3:
				reset(s);
				cout << "El sudoku ha sido restaurado correctamente";
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
			case 5: 
				autocompleta(s);
				cout << "Las posiciones VACIAS del sudoku con un unico valor se han completado correctamente";
				break;
			}
			mostrar_juego_consola(s);

			if (terminado(s)) {
				cout << "ENHORABUENA USTED A LOGRADO COMPLETAR EL SUDOKU!!" << endl;
				opcion = 6;
			}
			else {
				opcion = menu();
			}
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
	cout << "Introduce la fila: "<<endl;
	cin >> pos.fila;
	cout << "Introduce la columna: " << endl;
	cin >> pos.columna;

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


void valores_posibles(const tSudoku& s, tPosicion pos) {
	cout << "Los valores posibles son: ";
	for (int i = 1; i < 10; i++) {
		if (es_valor_posible(s, pos, i) == true) {
			cout << i << " ";
		}
	}
	cout << "\n";
}


//paso 4
