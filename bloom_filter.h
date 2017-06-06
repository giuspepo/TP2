/*
 * bloom_filter.h
 *
 *  Created on: 6 de jun. de 2017
 *      Author: algoritmos2
 */

#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_
#include <stddef.h>
#include <stdbool.h>

typedef struct bloom_filter bloom_filter_t;
typedef size_t (*hash_func_t) (const char*, size_t);

bloom_filter_t* bloom_filter_crear(hash_func_t funcion);

bool bloom_filter_cargar(bloom_filter_t* filtro, const char* clave);

void bloom_filter_destruir(bloom_filter_t* filtro);

#endif /* BLOOM_FILTER_H_ */
