#ifndef LIB_H_
#define LIB_H_

#include "hash.h"
#include "heap.h"
#include "bloom_filter.h"
#include <stddef.h>
#include <stdbool.h>


size_t hashing1(const char* clave, size_t tam);

size_t hashing2(const char* clave, size_t tam);

size_t hashing3(const char* clave, size_t tam);

int comparador_minimos(const void* a, const void* b);

heap_t* obtener_tts(hash_t* hash, bloom_filter_t* filtro, size_t tope);

void imprimir_tts(heap_t* heap);

void imprimir_usuarios(heap_t* heap);

#endif /* LIB_H_ */
