all : main.o draw.o
	gcc main.o draw.o -lncurses

main.o : main.c movement.h
	gcc -c main.c -lncurses

draw.o : draw.c draw.h
	gcc -c draw.c -lncurses

run : a.out
	./a.out

clean :
	rm a.out
