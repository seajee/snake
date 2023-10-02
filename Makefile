
CC=gcc
CFLAGS=-Wall -Wextra
LIBS=`pkg-config --libs ncurses`
SRC=src/main.c src/vec.c src/game.c src/util.c src/screen.c

main:
	$(CC) $(CFLAGS) $(LIBS) $(SRC) -o snake
