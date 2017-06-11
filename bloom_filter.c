/*
 * bloom_filter.c
 *
 *  Created on: 6 de jun. de 2017
 *      Author: algoritmos2
 */
#include "bloom_filter.h"
#include <stdlib.h>
#define TAM_VECTOR 4115226 //Un numero arbitrario. Lo podemos cambiar luego.

struct bloom_filter {
	size_t* arreglo1; 
	hash_func_t funcion_hash1;
	size_t* arreglo2;
	hash_func_t funcion_hash2;
	size_t* arreglo3; 
	hash_func_t funcion_hash3;
};

size_t minimo(size_t a, size_t b, size_t c) {
	size_t menor;
	if (a < b) menor = a; 
	else menor = b;
	
	if (menor < c) return menor;
	return c;
}
	
	
bloom_filter_t* bloom_filter_crear(hash_func_t funcion1, hash_func_t funcion2, hash_func_t funcion3) {
	bloom_filter_t* filtro = malloc(sizeof(bloom_filter_t));
	if (filtro == NULL) return NULL;

	//Calloc. Asi todos los elementos son iniciados con 0.
	filtro->arreglo1 = calloc(TAM_VECTOR,sizeof(size_t));
	if (filtro->arreglo1 == NULL) {
		free(filtro);
		return NULL;
	}

	filtro->arreglo2 = calloc(TAM_VECTOR,sizeof(size_t));
	if (filtro->arreglo2 == NULL) {
		free(filtro->arreglo1);
		free(filtro);
		return NULL;
	}
	filtro->arreglo3 = calloc(TAM_VECTOR,sizeof(size_t));
	filtro->arreglo3 = calloc(TAM_VECTOR,sizeof(size_t));
	if (filtro->arreglo3 == NULL) {
		free(filtro->arreglo2);
		free(filtro->arreglo1);
		free(filtro);
		return NULL;
	}
	filtro->tam = MAXIMO;
	filtro->funcion_hash1 = funcion1;
	filtro->funcion_hash2 = funcion2;
	filtro->funcion_hash3 = funcion3;
	
	return filtro;
}

bool bloom_filter_cargar(bloom_filter_t* filtro, const char* clave) {
	if (filtro == NULL || clave == NULL) return false;
	size_t pos1 = filtro->funcion_hash(clave, TAM_VECTOR);
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

