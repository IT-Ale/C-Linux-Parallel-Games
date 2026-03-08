game: main.o alieno.o difese.o 
	gcc main.o alieno.o difese.o -o game -lncurses

main.o: main.c
	gcc -c main.c

alieno.o: alieno.c
	gcc -c alieno.c

difese.o: difese.c
	gcc -c difese.c

clean:
	rm -f *.o game
