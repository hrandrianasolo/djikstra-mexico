CC=gcc
CFLAGS=-O2 -Wall
LINK=-O2
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

dijkstra: 	$(OBJ)
	$(CC) $(OBJ) $(LINK) -o $@

clean:
	rm *.o dijkstra