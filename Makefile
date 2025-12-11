CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

all: portfolio

portfolio: main.c
	$(CC) $(CFLAGS) -o portfolio main.c -lm

clean:
	rm -f portfolio
