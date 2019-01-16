all: main.o guest.o networking.o diff.o draw.o movement.o map.o player.h
	gcc -o play_host  main.o  networking.o diff.o draw.o movement.o map.o -lncursesw
	gcc -o play_guest guest.o networking.o diff.o draw.o movement.o map.o -lncursesw

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
	rm play_host play_guest

host:
	./play_host

guest:
	./play_guest $(ARGS)
