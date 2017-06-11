/*
 * procesar_usuarios.c
 *
 *  Created on: 5 de jun. de 2017
 *      Author: algoritmos2
 */

#include "bloom_filter.h"			
#include "hash.h"			
#include "strutil.h"			
#include <stdio.h>			
#include <stdlib.h>			
#include <string.h>			
			
int main(int argc, char* argv[]) {			
	if (argc < 2) return 1;		
			
	FILE* archivo = fopen(argv[1], "r");		
	if (archivo == NULL) return -1;		
			
	//Creamos los TDAs		
	hash_t* usuarios = hash_crear(NULL);		
	//if usuarios error		
	bloom_filter_t* filtro1 = bloom_filter_crear(hashing1);		
	bloom_filter_t* filtro2 = bloom_filter_crear(hashing2);		
	bloom_filter_t* filtro3 = bloom_filter_crear(hashing3);		
	//If filtros error		
			
	char buffer = NULL;		
	int tam = 0;		
			
	while (getline(&buffer,&tam,archivo) != -1) {		
		char** arreglo = split(buffer, ',');	
		//if error	
		if (!hash_pertenece(usuarios, arreglo[0])) hash_guardar(usuarios, arreglo[0], NULL);	
		int j = 1; //se encarga de leer los tags.	
		while (arreglo[j] != NULL) {	
			bloom_filter_cargar(filtro1, arreglo[0]);
			bloom_filter_cargar(filtro2, arreglo[0]);
			bloom_filter_cargar(filtro3, arreglo[0]);
		}	
		free_strv(arreglo);	
	}		
			
	//Imprimir los elementos en los arreglos.		
			
	free(buffer);		
	fclose(archivo);		
			
	return 0;		
}			
