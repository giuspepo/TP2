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
#include "lib.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define EXIT_FAILED -1
#define EXIT_SUCCESS 0

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
			j++;
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
			//Creamos un nuevo hash, completamente vacio.
			hash_t* tags2 = tags;
			tags = hash_crear(NULL);
			if (!tags) {
				hash_destruir(tags2);
				bloom_filter_destruir(filtro);
				free(buffer);
				fclose(stdin);
				return EXIT_FAILED;
			}
			hash_destruir(tags2);
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
