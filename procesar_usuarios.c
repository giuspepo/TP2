/*
 * procesar_usuarios.c
 *
 *  Created on: 5 de jun. de 2017
 *      Author: algoritmos2
 */
#define _POSIX_C_SOURCE 200809L
#include "bloom_filter.h"
#include "hash.h"
#include "lib.h"
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_FAILED -1
#define EXIT_SUCCESS 0

int cargar_usuarios(const char* filename) {
	//Creamos los TDAs
	FILE* archivo = fopen(filename, "r");		
	if (archivo == NULL) return EXIT_FAILED;

	hash_t* hash = hash_crear(NULL);
	if (!hash) {
		fclose(archivo);
		return EXIT_FAILED;
	}
	//Estos son los bloom filters en los que se almacenarán las cuentas de tags.
	bloom_filter_t* filtro = bloom_filter_crear(hashing1,hashing2,hashing3);
	if (!filtro){
		hash_destruir(hash);
		return EXIT_FAILED;
	}
	
	char* buffer = NULL;		
	size_t tam = 0;		
			
	while (getline(&buffer,&tam,archivo) != -1) {		
		char** arreglo = split(buffer, ',');	
		if (!arreglo){
 			free(buffer);
 			hash_destruir(hash);
 			bloom_filter_destruir(filtro);
 			fclose(archivo);
 			return EXIT_FAILED;
 		}
		if (!hash_pertenece(hash, arreglo[0])) hash_guardar(hash, arreglo[0], NULL);	
		int j = 1; //se encarga de leer los tags.	
		while (arreglo[j] != NULL) {	
			bloom_filter_cargar(filtro, arreglo[0]);
			j++;
		}
		free_strv(arreglo);	
	}
	heap_t* heap = obtener_tts(hash, filtro, hash_cantidad(hash));		
	//Si el heap no se crea imprimir usuarios no hace nada.
	imprimir_usuarios(heap);
	
	hash_destruir(hash);
	bloom_filter_destruir(filtro);
	free(buffer);		
	fclose(archivo);
	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {			
	if (argc < 2) return EXIT_FAILED;		
		
	int exit = cargar_usuarios(argv[1]);
	if (exit == EXIT_FAILED) {
		printf("ALGO SALIO MAL");
	}	
			
	return exit;		
}			
