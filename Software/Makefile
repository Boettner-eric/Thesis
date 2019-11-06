CC = gcc
flags = -Wall -o

all: test screen

test: test.c
	${CC} test.c aes.c ${flags} test

screen: screens.c
	${CC} screens.c aes.c ${flags} screen

clean:
	rm -f *.out
	rm test
	rm screen
	clear
