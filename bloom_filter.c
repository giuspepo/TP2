/*
 * bloom_filter.c
 *
 *  Created on: 6 de jun. de 2017
 *      Author: algoritmos2
 */
#include "bloom_filter.h"
#include <stdlib.h>
#define TAM_VECTOR 1000 //Un numero arbitrario. Lo podemos cambiar luego.

/* El filtro guarda tres arreglos del mismo tamano,
 * Cada uno con su respectiva funcion de hash.
 */
struct bloom_filter {
	size_t* arreglo1; 
	hash_func_t funcion_hash1;
	size_t* arreglo2;
	hash_func_t funcion_hash2;
	size_t* arreglo3; 
	hash_func_t funcion_hash3;
};

//Devuelve el minimo de tres enteros size_t
size_t minimo(size_t a, size_t b, size_t c) {
	size_t menor; 			//crear un entero, O(1)
	if (a < b) menor = a; 	//comparar y asignar, O(1)
	else menor = b;			//asignar, O(1)
	
	if (menor < c) return menor; // O(1)
	return c; //O(1)
} //Orden de minimo: O(1)
	
/* Crea el filtro y sus tres arreglos.
 * Recibe tres funciones de hash para cada uno de los arreglos.
 */
bloom_filter_t* bloom_filter_crear(hash_func_t funcion1, hash_func_t funcion2, hash_func_t funcion3) {
	bloom_filter_t* filtro = malloc(sizeof(bloom_filter_t)); //O(1)
	if (filtro == NULL) return NULL; //O(1)

	//Calloc. Asi todos los elementos son inicializados a 0.
	filtro->arreglo1 = calloc(TAM_VECTOR,sizeof(size_t)); //O(1), supongo
	if (filtro->arreglo1 == NULL) {//free & return, O(1)
		free(filtro);
		return NULL;
	}

	filtro->arreglo2 = calloc(TAM_VECTOR,sizeof(size_t)); //O(1)
	if (filtro->arreglo2 == NULL) { //O(1)
		free(filtro->arreglo1);
		free(filtro);
		return NULL;
	}

	filtro->arreglo3 = calloc(TAM_VECTOR,sizeof(size_t));//O(1)
	if (filtro->arreglo3 == NULL) { //Son solo 3 free. O(1)
		free(filtro->arreglo2);
		free(filtro->arreglo1);
		free(filtro);
		return NULL;
	}

	filtro->funcion_hash1 = funcion1;
	filtro->funcion_hash2 = funcion2;
	filtro->funcion_hash3 = funcion3;
	
	return filtro;
} //Orden de crear: O(1)

/* Carga la aparicion de una clave al filtro.
 * Carga una aparicion a cada uno de los arreglos,
 * a la posicion dada por sus respectivas funciones de hash.
 * Devuelve false si encuentra algun error.
 */
bool bloom_filter_cargar(bloom_filter_t* filtro, const char* clave) {
	if (filtro == NULL || clave == NULL) return false;

	//funcion de hash. En esencia O(strlen(clave)).
	//Para efectos practicos, el largo de la clave es tratado como una constante
	size_t pos1 = filtro->funcion_hash1(clave, TAM_VECTOR); //O(1)
	filtro->arreglo1[pos1]++; //O(1)

	size_t pos2 = filtro->funcion_hash2(clave, TAM_VECTOR); //O(1)
	filtro->arreglo2[pos2]++;

	size_t pos3 = filtro->funcion_hash3(clave, TAM_VECTOR); //O(1)
	filtro->arreglo3[pos3]++;
	return true;
} //Orden de cargar: O(1)

/* Toma los tres valores almacenados para la clave
 * y devuelve el minimo de ellos.
 */
size_t bloom_filter_obtener(const bloom_filter_t* filtro, const char* clave) {
	if (filtro == NULL || clave == NULL) return 0; //O(1)

	size_t pos1 = filtro->funcion_hash1(clave, TAM_VECTOR); //O(1)
	size_t cant1 = filtro->arreglo1[pos1]; //Ubicar en vector O(1)

	size_t pos2 = filtro->funcion_hash2(clave, TAM_VECTOR); //O(1)
	size_t cant2 = filtro->arreglo2[pos2];

	size_t pos3 = filtro->funcion_hash3(clave, TAM_VECTOR);//O(1)
	size_t cant3 = filtro->arreglo3[pos3];

	return minimo(cant1, cant2, cant3); //O(1), como establecimos
} //Orden de obtener: O(1)

/* Vacia el filtro, para que pueda ser utilizado una vez mas.
void bloom_filter_vaciar(bloom_filter_t* filtro) {
	for (size_t i = 0; i < TAM_VECTOR; i++) { //O(tam_vector)
		filtro->arreglo1[i] = 0; //O(1)
		filtro->arreglo2[i] = 0; //O(1)
		filtro->arreglo3[i] = 0; //O(1)
	}
} //Orden de vaciar: O(tam_vector);
//Esta funcion se creo para evitar que se tengan que eliminar y crear de nuevo
 * otro bloom_filter.*/

/* ELimina el filtro y todos los elementos. */
void bloom_filter_destruir(bloom_filter_t* filtro) {
	free(filtro->arreglo1);
	free(filtro->arreglo2);
	free(filtro->arreglo3);
	
	free(filtro);
}
