/*
 * procesar_tweets.c
 *
 *  Created on: 5 de jun. de 2017
 *      Author: algoritmos2
 */
#include "bloom_filter.h"
#include "hash.h"
#include "strutil.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

size_t hashing1(const char* clave, size_t tam){
	size_t hash = 0;
	int c;		
	while (c = *str++){		
		hash = (hash << 13) + (hash << 25) - hash + c; 	
	}		
	return hash % tam;		
}			
			
size_t hashing2(const char* clave, size_t tam){			
	size_t len = strlen ( clave );		
	size_t hash = 0;		
	for (int i = 0; i < len; i++)		
		hash = hash + str[i];	
	return hash % tam;		
}			
			
size_t hashing3(const char* clave, size_t tam){			
	size_t hash = 5381;		
	int c;		
	while (c = *str++){		
		hash = (hash << 5) + hash + c; 	
	}		
	return hash % tam;		
}
heap_t* obtener_tts(hash_t* hash, bloom_filter_t* filtro, size_t tope, int (*cmp) (void*, void*)) {
	heap_t* heap = heap_crear(cmp,NULL);
	if (heap == NULL) return NULL;
	
	hash_iter_t * iterador = hash_iter_crear(hash);
	if (iterador == NULL) {
		heap_destruir(heap);
		return NULL;
	}
	
	while (!hash_iter_al_final(iterador)) {
		//estructura que guarda el tag y la cantidad de veces
		//que aparecio, devuelta por el filtro.
		//Guardar la estructura al heap
		//If (heap_cantidad > k) desencolar.
	}
}

//Procesar_tweets es su propio programa. Su función es main.
int main(int argc, char* argv[]) {	
  //Para validar
	if (argc < 3) return 1;		
			
	//El archivo es concatenado. Se lee como stdin.		
			
	int n = (int) strtol(argv[1], NULL, 10);		
	int k = (int) strtol(argv[2], NULL, 10);		
	//almacena los tags que se leen del archivo.		
	hash_t* tags = hash_crear(NULL);		
	//if tags error		
	//Estos son los bloom filters en los que se almacenarán las cuentas de tags.		
	bloom_filter_t* filtro1 = bloom_filter_crear(hashing1);		
	bloom_filter_t* filtro2 = bloom_filter_crear(hashing2);		
	bloom_filter_t* filtro3 = bloom_filter_crear(hashing3);		
	//If filtros error		
			
	char buffer = NULL;		
	int tam = 0;		
	int i = 0; //Cuenta los parrafos hasta llegar a n.		
	while (getline(&buffer,&tam,stdin) != -1) {		
		char** arreglo = split(buffer, ',');	
		//if error	
		int j = 1; //se encarga de leer los tags.	
		while (arreglo[j] != NULL) {	
			if (!hash_pertenece(tags, arreglo[j])) hash_guardar(tags, arreglo [j], NULL);
			bloom_filter_cargar(filtro1, arreglo[j]);
			bloom_filter_cargar(filtro2, arreglo[j]);
			bloom_filter_cargar(filtro3, arreglo[j]);
		}	
		free_strv(arreglo);	
		i++;	
		if (i == n) {	
			heap_t* trends = obtener_tts(tags, filtro1, filtro2, filtro3, k, comparador_minimos);
			// imprimir
			// vaciar las tablas y el heap
			i = 0;
		}	
	}		
	if (i != 0) {		
		heap_t trends = obtener_tts(tags, filtro1, filtro2, filtro3, k, comparador_minimos);	
		// imprimir	
		// vaciar las tablas y el heap	
	}		
			
	free(buffer);		
	fclose(stdin);		
			
	return 0;		
}			
			
