game: main.o alieno.o thread_functions.o difese.o
	gcc main.o alieno.o thread_functions.o difese.o -o game -lncurses -pthread

main.o: main.c
	gcc -c main.c

alieno.o: alieno.c
	gcc -c alieno.c
	
thread_functions.o: thread_functions.c
	gcc -c thread_functions.c

difese.o: difese.c
	gcc -c difese.c

clean:
	rm -f *.o game
