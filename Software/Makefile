CC = gcc

all: test screen

test: test.c
	${CC} test.c aes.c -o test

screen: screens.c
	${CC} screens.c aes.c -o screen

clean:
	rm -f *.out
	rm test
	rm screen
	clear
