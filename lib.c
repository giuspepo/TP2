#define _POSIX_C_SOURCE 200809L
#include "bloom_filter.h"
#include "lib.h"
#include "hash.h"
#include "strutil.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct tt_nodo {
	const char* clave;
	size_t cant;
}tt_nodo_t;

size_t hashing1(const char* clave, size_t tam){
	size_t hash = 0;
	int i = 0;
	while (i < strlen(clave)) {
		hash = (hash << 13) + (hash << 25) - hash + clave[i];
		i++;
	}		
	return hash % tam;		
}			
			
size_t hashing2(const char* clave, size_t tam){			
	size_t len = strlen ( clave );		
	size_t hash = 0;		
	for (int i = 0; i < len; i++)		
		hash = hash + clave[i];
	return hash % tam;		
}			
			
size_t hashing3(const char* clave, size_t tam){			
	size_t hash = 5381;		
	int i = 0;
	while (i < strlen(clave)){
		hash = (hash << 5) + hash + clave[i];
		i++;
	}		
	return hash % tam;		
}

int comparador_minimos(const void* a, const void* b) {
	tt_nodo_t* tag_a = (tt_nodo_t*) a;
	tt_nodo_t* tag_b = (tt_nodo_t*) b;

	return ((int) tag_b->cant) - ((int) tag_a->cant);
}

heap_t* obtener_tts(hash_t* hash, bloom_filter_t* filtro, size_t tope) {
	heap_t* heap = heap_crear(comparador_minimos);
	if (heap == NULL) return NULL;
	
	hash_iter_t * iterador = hash_iter_crear(hash);
	if (iterador == NULL) {
		heap_destruir(heap,free);
		return NULL;
	}
	
	while (!hash_iter_al_final(iterador)) {

		const char* clave = hash_iter_ver_actual(iterador);
		tt_nodo_t* tt_nodo = malloc(sizeof(tt_nodo_t));
		if (tt_nodo == NULL) break;
		tt_nodo->clave = clave;
		tt_nodo->cant = bloom_filter_obtener(filtro, clave);

		if (!heap_encolar(heap, tt_nodo)) {
			free(tt_nodo);
			break;
		}
		if (heap_cantidad(heap) > tope) {
			tt_nodo_t* minimo = heap_desencolar(heap);
			free(minimo);
		}
		hash_iter_avanzar(iterador);
	}
	hash_iter_destruir(iterador);
	return heap;
}

void imprimir_tts(heap_t* heap){
	if (heap == NULL) return;
	size_t tope = heap_cantidad(heap);
	printf("** %d TTS HISTORICOS  **\n", tope);
	tt_nodo_t** arreglo = malloc(tope * sizeof(tt_nodo_t*));
	if (arreglo == NULL) {
		heap_destruir(heap,free);
		return;
	}

	for (size_t i = 0; i < tope; i++){
		tt_nodo_t* nodo = heap_desencolar(heap);
		arreglo[i] = nodo;
	}

	for (int j = 0; j < tope; j++) {
		tt_nodo_t* nodo = arreglo[tope-j-1];
		printf("TT: %s, Cant: %d\n", nodo->clave, nodo->cant);
		free(nodo);
	}
	free(arreglo);
	heap_destruir(heap, free);
}

void imprimir_usuarios(heap_t* heap){
	if (heap == NULL) return;

	printf("** USUARIOS POR HASHTAGS  **\n");

	//Contador de tags. Guarda un numero para imprimir 
	//los usuarios que usaron dicho total de tags.
	size_t tagc = 0;
	while (!heap_esta_vacio(heap)) {
		tt_nodo_t* nodo = heap_desencolar(heap);
		if (tagc < nodo->cant) {
			printf("\n%zu: ", nodo->cant);
			tagc = nodo->cant;
		}
		//El heap devuelve elementos por cantidad, de menor a mayor.
		//Si el que sigue no es mayor, obligatoriamente es igual.
		else printf(", "); //La coma separadora.
		printf("%s", nodo->clave);
		free(nodo);
	}
	heap_destruir(heap, free);
}

