#ifndef celda_h
#define celda_h

// 
enum tEstado {ORIGINAL, OCUPADA, VACIA};
struct tCelda {
	int valor; // Valor existente en la celda
	tEstado estado;	// Estado de la celda
};

// Operaciones de incializacion
void inicializaCelda(tCelda& c);	// Inicializa celda c a vacia: estado = vacia, valor = 0.
void inicializaCelda(tCelda& c, int v, tEstado estado);	// Inicializa celda con los valores pasados como parametros.

// Operaciones de consulta del estado
bool es_vacia(const tCelda& c);		// indica si la celda es vacia o no
bool es_original(const tCelda& c);		// indica si la celda es original o no
bool es_ocupada(const tCelda& c);	// indica si la celda es ocupada o no
int dame_valor(const tCelda& c);	// Devuelve el valor existente en dicha celda

// Operaciones de modificacion del estado 
void pon_valor(tCelda& c, int v);	// pone v como nuevo valor de la celda
void pon_vacia(tCelda& c);			// pone la celda a vacia
void pon_original(tCelda& c);		// pone la celda a original
void pon_ocupada(tCelda& c);		// pone la celda a ocupada

#endif 