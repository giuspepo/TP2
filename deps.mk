procesar_tweets: procesar_tweets.o hash.o heap.o strutil.o

procesar_usuarios: procesar_usuarios.o hash.o strutil.o


procesar_tweets.o: procesar_tweets.c

procesar_usuarios.o: procesar_usuarios.c

hash.o: hash.c hash.h

heap.o: heap.c heap.h

strutil.o: strutil.c strutil.h
