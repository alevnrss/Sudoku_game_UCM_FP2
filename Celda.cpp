#include "Celda.h"


// Operaciones de inicializacion
void inicializaCelda(tCelda& c) {
	c.estado = VACIA;
	c.valor = 0;
}

void inicializaCelda(tCelda& c, int v, tEstado estado) {
	c.estado = estado;
	c.valor = v;
}

// Operaciones de consulta del estado
bool es_vacia(const tCelda& c) {
	return (c.estado == VACIA) ? true : false;
}

bool es_original(const tCelda& c) {
	return (c.estado == ORIGINAL) ? true : false;
}
bool es_ocupada(const tCelda& c) {
	return (c.estado == OCUPADA) ? true : false;
}

int dame_valor(const tCelda& c) {
	return c.valor;
}

// Operaciones de modificacion del estado
void pon_valor(tCelda& c, int v) {
	c.valor = v;
}
void pon_vacia(tCelda& c) {
	c.estado = VACIA;
	c.valor = 0;
}
void pon_original(tCelda& c) {
	c.estado = ORIGINAL;
}
void pon_ocupada(tCelda& c) {
	c.estado = OCUPADA;
}


