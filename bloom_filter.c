/*
 * bloom_filter.c
 *
 *  Created on: 6 de jun. de 2017
 *      Author: algoritmos2
 */
#include "bloom_filter.h"
#include <stdlib.h>
#define MAXIMO 12345679 //Un numero arbitrario. Lo podemos cambiar luego.

struct bloom_filter {
	size_t* arreglo;
	size_t tam;
	hash_func_t funcion_hash;
};

bloom_filter_t* bloom_filter_crear(hash_func_t funcion) {
	bloom_filter_t* filtro = malloc(sizeof(bloom_filter_t));
	if (filtro == NULL) return NULL;

	//Calloc. Asi todos los elementos son iniciados con 0.
	filtro->arreglo = calloc(MAXIMO,sizeof(size_t));
	if (filtro->arreglo == NULL) {
		free(filtro);
		return NULL;
	}

	filtro->tam = MAXIMO;
	filtro->funcion_hash = funcion;

	return filtro;
}

bool bloom_filter_cargar(bloom_filter_t* filtro, const char* clave) {
	if (filtro == NULL || clave == NULL) return false;
	size_t pos = filtro->funcion_hash(clave, filtro->tam);
	filtro->arreglo[pos]++;
	return true;
}

size_t bloom_filter_obtener(bloom_filter_t* filtro, const char* clave) {
	if (filtro == NULL || clave == NULL) return 0;
	size_t pos = filtro->funcion_hash(clave, filtro->tam);
	return filtro->arreglo[pos];
}

void bloom_filter_destruir(bloom_filter_t* filtro) {
	free(filtro->arreglo);
	free(filtro);
}

