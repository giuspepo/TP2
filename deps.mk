procesar_tweets: procesar_tweets.o bloom_filter.o hash.o lista.o heap.o pila.o strutil.o

procesar_usuarios: procesar_usuarios.o bloom_filter.o hash.o lista.o strutil.o


procesar_tweets.o: procesar_tweets.c

procesar_usuarios.o: procesar_usuarios.c

bloom_filter.o: bloom_filter.h bloom_filter.c

hash.o: hash.h hash.c

lista.o: lista.h lista.c

heap.o: heap.h heap.c

pila.o: pila.h pila.c 

strutil.o: strutil.h strutil.c
