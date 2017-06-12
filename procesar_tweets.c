/*
 * procesar_tweets.c
 *
 *  Created on: 5 de jun. de 2017
 *      Author: algoritmos2
 */
#define _POSIX_C_SOURCE 200809L
#include "bloom_filter.h"
#include "hash.h"
#include "strutil.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define EXIT_FAILED -1
#define EXIT_SUCCESS 0

size_t hashing1(const char* clave, size_t tam){
	size_t hash = 0;
	int i = 0;//int c;
	while (i < strlen(clave)) { //while (c = *clave++) {
		hash = (hash << 13) + (hash << 25) - hash + clave[i];
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
	}		
	return hash % tam;		
}

typedef struct tt_nodo {
	const char* tag;
	size_t cant;
}tt_nodo_t;

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
		tt_nodo_t* tt_nodo = malloc(sizeof(tt_nodo));
		if (tt_nodo == NULL) break;
		tt_nodo->tag = clave;
		tt_nodo->cant = bloom_filter_obtener(filtro, clave);

		if (!heap_encolar(heap, tt_nodo)) {
			free(tt_nodo);
			break;
		}
		if (heap_cantidad(heap) > tope) {
			tt_nodo_t* minimo = heap_desencolar(heap);
			free(minimo);
		}
	}
	hash_iter_destruir(iterador);
	return heap;
}

void imprimir_tts(heap_t* heap){

	size_t tope = heap_cantidad(heap);
	printf("** %d TTS HISTORICOS  **\n", tope);
	//Este arreglo guarda el total de elementos en el heap, que se asume que es k
	//El heap guarda los elementos con el minimo al frente, para facilitar retirarlo.
	//Dicho esto, los TT deben imprimirse de mayor a menor.
	//Este arreglo funciona como una pila, sin necesidad de crear una pila.
	//Guardamos los elementos de menor a mayor, e imprimimos desde el ultimo.
	tt_nodo_t** arreglo = malloc(tope*sizeof(tt_nodo_t*));
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
		printf("TT: %s, Cant: %d\n", nodo->tag, nodo->cant);
		free(nodo);
	}
	free(arreglo);
	heap_destruir(heap, free);
}

int cargar_tts(int k, int n){
	//almacena los tags que se leen del archivo.
 	hash_t* tags = hash_crear(NULL);
	if (!tags) return EXIT_FAILED;

	//Estos son los bloom filters en los que se almacenarán las cuentas de tags.
	bloom_filter_t* filtro = bloom_filter_crear(hashing1,hashing2,hashing3);
	if (!filtro){
		hash_destruir(tags);
		return EXIT_FAILED;
	}

 	char* buffer = NULL;
	size_t tam = 0;
 	int i = 0; //Cuenta los parrafos hasta llegar a n.
	while (getline(&buffer,&tam,stdin) != -1) {
 		char** arreglo = split(buffer, ',');
 		//if error
 		int j = 1; //se encarga de leer los tags.
		while (arreglo[j] != NULL) {
 			if (!hash_pertenece(tags, arreglo[j])) hash_guardar(tags, arreglo [j], NULL);
 			bloom_filter_cargar(filtro, arreglo[j]);
		}
 		free_strv(arreglo);
		i++;
		if (i == n) {
			heap_t* trends = obtener_tts(tags, filtro, k);
			if (!trends){
				free(buffer);
				hash_destruir(tags);
				bloom_filter_destruir(filtro);
				fclose(stdin);
				return EXIT_FAILED;
			}
			imprimir_tts(trends); //destruye heap
			hash_destruir(tags);
			bloom_filter_destruir(filtro);
			tags = hash_crear(NULL);
			if (!tags) {
				free(buffer);
				fclose(stdin);
				return EXIT_FAILED;
			}
			//Vacia el filtro sin necesidad de destruirlo.
			bloom_filter_vaciar(filtro);
			i = 0;
		}
	}

	if (i != 0) {
		heap_t* trends = obtener_tts(tags, filtro, k);
		if (!trends){
		free(buffer);
			hash_destruir(tags);
			bloom_filter_destruir(filtro);
			fclose(stdin);
			return EXIT_FAILED;
		}
		imprimir_tts(trends); //destruye heap

	}
	hash_destruir(tags);
	bloom_filter_destruir(filtro);
	free(buffer);
	fclose(stdin);
	return EXIT_SUCCESS;
}

//Procesar_tweets es su propio programa. Su función es main.
int main(int argc, char* argv[]) {	
  //Para validar
	if (argc < 3) return EXIT_FAILED;
	//El archivo es concatenado. Se lee como stdin.		
	int n = (int) strtol(argv[1], NULL, 10);		
	int k = (int) strtol(argv[2], NULL, 10);		

	int exit = cargar_tts(k,n);
	if (exit == EXIT_FAILED){
		printf("ALGO SALIO MAL");
	}
	return exit;
}
