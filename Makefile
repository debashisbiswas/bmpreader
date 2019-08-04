EXEC = bmpedit
CC = gcc
CFLAGS = -Wall -g

$(EXEC) : main.c
	$(CC) $(CFLAGS) main.c -o $(EXEC)

clean  :
	rm ${EXEC}