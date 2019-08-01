EXEC = bmpedit
CC = gcc
CFLAGS = -Wall -g
LFLAGS = -lm

$(EXEC) : main.c
	$(CC) $(CFLAGS) main.c -o $(EXEC) $(LFLAGS)

clean  :
	rm ${EXEC}