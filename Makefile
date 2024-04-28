CC=gcc
CFLAGS=-g -Wall
ush: ush.o
	$(CC) $(CFLAGS) -o ush ush.o