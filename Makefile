
CC=gcc
CFLAGS=-Wall -Wextra
LIBS=`pkg-config --libs ncurses`
SRC=main.c

main:
	$(CC) $(CFLAGS) $(LIBS) $(SRC) -o snake
