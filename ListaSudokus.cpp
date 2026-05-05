#include "ListaSudokus.h"
#include <iostream>
using namespace std;

// SUBPROGRAMAS PRIVADOS
void redimensionar(tListaSudokus& lista) {
    lista.capacidad *= 2;
    tSudoku** nuevoArray = new tSudoku * [lista.capacidad];

    for (int i = 0; i < lista.cont; i++) {
        nuevoArray[i] = lista.sudokus[i];
    }

    delete[] lista.sudokus;
    lista.sudokus = nuevoArray;
}

// SUBPROGRAMAS PUBLICOS
void inicializa_lista_sudokus(tListaSudokus& lista) {
    lista.capacidad = MAX_LISTA;
    lista.sudokus = new tSudoku * [lista.capacidad]; // Reservamos el array de punteros
    lista.cont = 0;
}

void destruye_lista_sudokus(tListaSudokus& lista) {
    for (int i = 0; i < lista.cont; i++) {
        delete lista.sudokus[i];
        lista.sudokus[i] = nullptr;
    }

    delete[] lista.sudokus;
    lista.sudokus = nullptr;
    lista.cont = 0;
    lista.capacidad = 0;

}

// OPERACIONES DE CONSULTA DEL ESTADO
int dame_num_elems(const tListaSudokus& lista) {
    return lista.cont;
}
tSudoku* dame_sudoku(const tListaSudokus& lista, int i){
    return lista.sudokus[i];
}

// OPERACIONES DE MODIFICACION DEL ESTADO
void insertar(tListaSudokus& lista, tSudoku* s) {
    if (lista.cont == lista.capacidad) {
        redimensionar(lista);
    }

    int pos = 0; 
    while (pos < lista.cont && *lista.sudokus[pos] < *s) {
        pos++;
    }

    for (int i = lista.cont; i > pos; i--) {
        lista.sudokus[i] = lista.sudokus[i - 1];
    }

    lista.sudokus[pos] = s;
    lista.cont++;
}
void eliminar(tListaSudokus& lista, int pos) {
    if (pos >= 0 && pos < lista.cont) {
        delete lista.sudokus[pos];

        for (int i = pos; i < lista.cont - 1; i++) {
            lista.sudokus[i] = lista.sudokus[i + 1];
        }

        lista.sudokus[lista.cont - 1] = nullptr;
        lista.cont--;
    }
}

// OPERACIONES DE VISUALIZACION DEL ESTADO
void mostrar_lista(const tListaSudokus& lista) {
    for (int i = 0; i < lista.cont; i++) {
        
        int libres = dame_num_celdas_libres(*lista.sudokus[i]);
        cout << "Sudoku con " << libres << " casillas vacias" << endl;

        arrValores av;
        numero_posibles_valores(*lista.sudokus[i], av);
        for (int j = 0; j < DIM; j++) {
            cout << "  celdas con " << j + 1 << " valores posibles: " << av[j] << endl;
        }
        cout << endl;
    }
}