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

/* Lee un archivo con una lista de tweets. Dados dos enteros n y k, lee cada n
 * parrafos del archivo por vez y devuelve de ellos los k Trending Topics
 * y el maximo de veces que aparecieron.
 * El archivo es concatenado a la funcion, y se lee por stdin.
 */
int cargar_tts(int k, int n){

	//Controlador. Todo depende de que esta variable devuelva SUCCESS.
	int exit = EXIT_SUCCESS;

	//Tabla de hash. almacena los tags que se leen del archivo.
	//La ventaja de hash es que almacena y devuelve las claves en tiempo
	//practicamente O(1)
 	hash_t* tags = hash_crear(NULL); //O(1)
	if (!tags) exit = EXIT_FAILED;

	//Este es el filtro en el que se almacenarán las cuentas de los tags.
	bloom_filter_t* filtro = bloom_filter_crear(hashing1,hashing2,hashing3);
	if (!filtro) exit = EXIT_FAILED;

 	char* buffer = NULL;
	size_t tam = 0;
 	int i = 0; //Cuenta los parrafos hasta llegar a n.

 	//Como se puede ver en este bucle, todo depende que exit sea SUCCESS.
 	//Si algo falla, el bucle se corta.
	while (getline(&buffer,&tam,stdin) != -1 && exit == EXIT_SUCCESS) {
		//Elimina el retorno de carro, asi el hash no lo lee como una letra aparte
		buffer[strlen(buffer) - 1] = '\0';

 		char** arreglo = split(buffer, ',');
 		if (arreglo == NULL) {
 			exit = EXIT_FAILED;
 			break;
 		}
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
			if (!trends) {
				exit = EXIT_FAILED;
				break;
			}
			imprimir_tts(trends); //destruye heap
			//Creamos un nuevo hash, completamente vacio.
			hash_t* tags2 = tags;
			tags = hash_crear(NULL);
			hash_destruir(tags2);
			if (!tags) exit = EXIT_FAILED;

			bloom_filter_t* filtro2 = filtro;
			filtro = bloom_filter_crear(hashing1,hashing2,hashing3);
			bloom_filter_destruir(filtro2);
			if (!filtro) exit = EXIT_FAILED;

			i = 0;
		}
	}
	if (i != 0 && exit == EXIT_SUCCESS) {
		heap_t* trends = obtener_tts(tags, filtro, k);
		if (!trends) exit = EXIT_FAILED;
		imprimir_tts(trends); //destruye heap

	}
	if (tags != NULL) hash_destruir(tags);
	if (filtro != NULL) bloom_filter_destruir(filtro);
	if (buffer != NULL) free(buffer);
	fclose(stdin);
	return exit;
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
