all : main.o draw.o movement.o player.h
	gcc main.o draw.o movement.o -lncurses

main.o : main.c
	gcc -c main.c -lncurses

draw.o : draw.c draw.h
	gcc -c draw.c -lncurses

movement.o : movement.c movement.h
	gcc -c movement.c -lncurses

run : a.out
	./a.out

clean :
	rm a.out *.o
