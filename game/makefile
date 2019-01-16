all : main.o guest.o networking.o diff.o draw.o movement.o map.o player.h
	gcc -o host  main.o  networking.o diff.o draw.o movement.o map.o -lncursesw
	gcc -o guest guest.o networking.o diff.o draw.o movement.o map.o -lncursesw

guest.o: guest.c
	gcc -c guest.c

diff.o: diff.c diff.h
	gcc -c diff.c

networking.o: networking.c networking.h
	gcc -c networking.c

main.o: main.c
	gcc -c main.c

draw.o: draw.c draw.h
	gcc -c draw.c

movement.o: movement.c movement.h
	gcc -c movement.c

map.o: map.c map.h
	gcc -c map.c

clean:
	rm *.o
	rm host guest
	rm h2g g2h
