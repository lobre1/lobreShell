CC=gcc

all: install

install: main.c execution.c builtin.c
	$(CC) -Wall -Wextra -o lbsh main.c execution.c builtin.c

clean:
	rm lbsh
