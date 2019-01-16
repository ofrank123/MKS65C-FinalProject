all : main.o draw.o movement.o map.o player.h
	gcc main.o draw.o movement.o map.o -lncursesw

main.o : main.c
	gcc -c main.c

draw.o : draw.c draw.h
	gcc -c draw.c

movement.o : movement.c movement.h
	gcc -c movement.c

map.o: map.c map.h
	gcc -c map.c

run : a.out
	./a.out

clean :
	rm a.out *.o
