//NOMBRES Y APELLIDOS: Leiton Campillos y Alejandro Veneros
//GRUPO:7
#include "checkML.h"
#include "Celda.h"
#include "Tablero.h"
#include "Sudoku.h"
#include "InputOutput.h"
#include "ListaSudokus.h"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// Declaraciones de subprogramas privados
int menu();
void pide_pos(const tSudoku& s, int& fila, int& columna);
int pide_valor();
void ejecutar_opcion(int opcion, tSudoku& s);
bool jugar(tSudoku& s);
bool cargar_lista_sudokus_nuevos(string nombre_fichero, tListaSudokus& l);
bool cargar_lista_sudokus_comenzados(string nombre_fichero, tListaSudokus& ls);
void guardar_lista_sudokus_comenzados(string nombre_fichero, const tListaSudokus& ls);
char menu_principal();
int elegir_sudoku(const tListaSudokus& lista);


// menu_principal
char menu_principal() {
    char opcion = ' ';
    while (opcion != 'N' && opcion != 'C' && opcion != 'A') {
        cout << endl;
        cout << "Partida nueva (N), continuar partida (C) o abandonar la aplicacion (A)? ";
        cin >> opcion;
        if (opcion >= 'a' && opcion <= 'z') opcion = opcion - 'a' + 'A';
        if (opcion != 'N' && opcion != 'C' && opcion != 'A') {
            cout << "Opcion no valida. Elige N, C o A." << endl;
        }
    }
    return opcion;
}


int elegir_sudoku(const tListaSudokus& lista) {
    mostrar_lista(lista);
    int eleccion = 0;
    int n = dame_num_elems(lista);
    while (eleccion < 1 || eleccion > n) {
        cout << "Elige un Sudoku (1-" << n << "): ";
        cin >> eleccion;
        if (eleccion < 1 || eleccion > n) {
            cout << "Posicion no valida." << endl;
        }
    }
    return eleccion - 1; 
}


int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // --- Carga de las dos listas ---
    tListaSudokus lista_nuevos;
    tListaSudokus lista_comenzados;
    inicializa_lista_sudokus(lista_nuevos);
    inicializa_lista_sudokus(lista_comenzados);

    // Si cualquiera de las dos cargas falla, la aplicacion termina
    bool cargaOk = cargar_lista_sudokus_nuevos("lista_sudokus.txt", lista_nuevos);
    if (!cargaOk) {
        cout << "Error: no se pudo abrir el fichero de Sudokus nuevos." << endl;
    }

    if (cargaOk && !cargar_lista_sudokus_comenzados("lista_comenzados.txt", lista_comenzados)) {
        cout << "Error: no se pudo abrir el fichero de Sudokus comenzados." << endl;
        cargaOk = false;
    }

    // --- Bucle principal ---
    char opcionMenu = ' ';
    while (cargaOk && opcionMenu != 'A') {
        opcionMenu = menu_principal();

        if (opcionMenu != 'A') {

            // Determinar si jugamos nuevo o comenzado
            bool juegoNuevo;

            if (opcionMenu == 'C') {
                if (dame_num_elems(lista_comenzados) == 0) {
                    cout << "No hay Sudokus comenzados. Debes jugar un Sudoku nuevo." << endl;
                    juegoNuevo = true;
                }
                else {
                    juegoNuevo = false;
                }
            }
            else { // 'N'
                juegoNuevo = true;
            }

            // Elegir el sudoku de la lista correspondiente
            int pos;

            if (juegoNuevo) {
                cout << endl << "--- Sudokus nuevos disponibles ---" << endl;
                pos = elegir_sudoku(lista_nuevos);
            }
            else {
                cout << endl << "--- Sudokus comenzados disponibles ---" << endl;
                pos = elegir_sudoku(lista_comenzados);
            }

            // --- Jugar ---
            bool abandonado;

            if (juegoNuevo) {
                tSudoku* original = dame_sudoku(lista_nuevos, pos);
                // Creamos copia para jugar: el original permanece intacto en lista_nuevos
                tSudoku* copiaJuego = new tSudoku(*original);

                abandonado = jugar(*copiaJuego);

                if (abandonado) {
                    // Insertar la copia en su estado actual en lista_comenzados
                    insertar(lista_comenzados, copiaJuego);
                    cout << "Progreso guardado en la lista de partidas comenzadas." << endl;
                }
                else {
                    // Completado: liberamos la copia (el original sigue en lista_nuevos)
                    delete copiaJuego;
                    cout << "Sudoku completado. Puedes volver a jugarlo desde cero." << endl;
                }
            }
            else {
                // Sudoku comenzado: primero lo sacamos de la lista con una copia
                tSudoku* original = dame_sudoku(lista_comenzados, pos);
                tSudoku* copiaJuego = new tSudoku(*original);

                // eliminar() hace delete del puntero en la lista, la copia es independiente
                eliminar(lista_comenzados, pos);

                abandonado = jugar(*copiaJuego);

                if (abandonado) {
                    // Reinsertarlo en su estado actual (posicion ordenada segun dificultad)
                    insertar(lista_comenzados, copiaJuego);
                    cout << "Progreso guardado en la lista de partidas comenzadas." << endl;
                }
                else {
                    // Completado: se elimina definitivamente
                    delete copiaJuego;
                    cout << "Enhorabuena, Sudoku completado y eliminado de la lista." << endl;
                }
            }
        }
    }

    //al cerrar: guardar lista de comenzados (solo si la carga fue exitosa)
    if (cargaOk) {
        guardar_lista_sudokus_comenzados("lista_comenzados.txt", lista_comenzados);
    }



    destruye_lista_sudokus(lista_nuevos);
    destruye_lista_sudokus(lista_comenzados);

    cout << "Hasta pronto." << endl;
    return 0;
}

// menu de juego
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

// pide_pos: pide fila y columna validas segun la dimension del sudoku.
void pide_pos(const tSudoku& s, int& fila, int& columna) {
    int dim = dame_dimension(s);
    fila = 0;
    columna = 0;
    while (fila < 1 || fila > dim) {
        cout << "Introduce la fila (1-" << dim << "): ";
        cin >> fila;
        if (fila < 1 || fila > dim) {
            cout << "Fila no valida, introducela de nuevo." << endl;
        }
    }
    while (columna < 1 || columna > dim) {
        cout << "Introduce la columna (1-" << dim << "): ";
        cin >> columna;
        if (columna < 1 || columna > dim) {
            cout << "Columna no valida, introducela de nuevo." << endl;
        }
    }
    fila = fila - 1;
    columna = columna - 1;
}

// pide_valor
int pide_valor() {
    int valor;
    cout << "Introduce el valor: ";
    cin >> valor;
    return valor;
}


// ejecutar_opcion
void ejecutar_opcion(int opcion, tSudoku& s) {
    int fila, columna, valor;

    switch (opcion) {
    case 1: // Poner valor
        pide_pos(s, fila, columna);
        valor = pide_valor();
        if (!pon_valor_sudoku(s, fila, columna, valor))
            cout << "Movimiento no permitido." << endl;
        break;
    case 2: // Quitar valor
        pide_pos(s, fila, columna);
        if (!quitar_valor_sudoku(s, fila, columna))
            cout << "No se puede quitar ese valor." << endl;
        break;
    case 3: // Reset
        reset(s);
        cout << "Sudoku reiniciado." << endl;
        break;
    case 4: // Valores posibles
        pide_pos(s, fila, columna);
        valores_posibles(s, { fila, columna });
        break;
    case 5: // Autocompletar
        autocompleta(s);
        break;
    }
}

bool cargar_lista_sudokus_nuevos(string nombre_fichero, tListaSudokus& l) {
    bool exito = false;
    ifstream archivo;
    archivo.open(nombre_fichero);

    if (archivo.is_open()) {
        int numSudokus;
        archivo >> numSudokus;

        for (int i = 0; i < numSudokus; i++) {
            string nombreSudoku;
            archivo >> nombreSudoku;
            ifstream fSudoku;
            fSudoku.open(nombreSudoku);

            if (fSudoku.is_open()) {
                tSudoku* s = new tSudoku;
                carga_sudoku(fSudoku, *s);
                insertar(l, s);
                fSudoku.close();
            }
        }
        archivo.close();
        exito = true;
    }

    return exito;
}

bool cargar_lista_sudokus_comenzados(string nombre_fichero, tListaSudokus& ls) {
    bool exito = false;
    ifstream archivo;
    archivo.open(nombre_fichero);

    if (archivo.is_open()) {
        int numSudokus;
        // Si el archivo existe pero esta vacio (0 sudokus), se carga correctamente
        if (archivo >> numSudokus) {
            for (int i = 0; i < numSudokus; i++) {
                tSudoku* s = new tSudoku;
                if (carga_sudoku(archivo, *s)) {
                    int fila;
                    archivo >> fila;
                    while (fila != -1) {
                        int col, valor;
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
        }
        archivo.close();
        exito = true;
    }

    return exito;
}

void guardar_lista_sudokus_comenzados(string nombre_fichero, const tListaSudokus& ls) {
    ofstream archivo(nombre_fichero);
    if (archivo.is_open()) {
        archivo << dame_num_elems(ls) << endl;
        for (int i = 0; i < dame_num_elems(ls); i++) {
            tSudoku* s = dame_sudoku(ls, i);
            int dim = dame_dimension(*s);

            // Tablero: solo los valores originales; el resto como 0
            archivo << dim << endl;
            for (int f = 0; f < dim; f++) {
                for (int c = 0; c < dim; c++) {
                    tCelda celda = dame_celda(*s, f, c);
                    if (es_original(celda))
                        archivo << dame_valor(celda) << " ";
                    else
                        archivo << "0 ";
                }
                archivo << endl;
            }

            // Celdas puestas por el jugador (estado OCUPADA)
            for (int f = 0; f < dim; f++) {
                for (int c = 0; c < dim; c++) {
                    tCelda celda = dame_celda(*s, f, c);
                    if (es_ocupada(celda)) {
                        archivo << f << " " << c << " "
                            << dame_valor(celda) << endl;
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
            cout << "Has cometido un error previo. El Sudoku no tiene solucion actual." << endl;
            cout << "Debes quitar algun valor para desbloquear las casillas: ";
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
            ejecutar_opcion(opcion, s);
        }
    }

    if (terminado(s)) {
        mostrar_juego_consola(s);
        cout << "Sudoku completado con exito." << endl;
    }

    return abandonado;
}