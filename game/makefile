all : main.o guest.o diff.o draw.o movement.o map.o player.h
	gcc -o host main.o diff.o draw.o movement.o map.o -lncursesw
	gcc -o guest guest.o diff.o draw.o movement.o map.o -lncursesw

guest.o: guest.c
	gcc -c guest.c

diff.o: diff.c diff.h

main.o: main.c
	gcc -c main.c

draw.o: draw.c draw.h
	gcc -c draw.c

movement.o: movement.c movement.h
	gcc -c movement.c

map.o: map.c map.h
	gcc -c map.c

run: a.out
	./a.out

clean:
	rm a.out *.o
