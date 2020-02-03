CC = gcc
flags = -Wall -Werror -O3 -o
shared = -shared -o
object = -c -Wall -Werror -fpic -Wextra -pedantic -O3

all: screen test

test: aes.c
	${CC} ${object} aes.c
	${CC} ${shared} aes.so aes.o
	python3 -m unittest -v

screen: screens.c
	${CC} screens.c aes.c ${flags} screen

clean:
	rm -f *.so *.o *.out screen
	rm -R __pycache__
