CC = gcc
flags = -Wall -Werror -O3 -o
shared = -shared -o
object = -c -Wall -Werror -fpic -Wextra -pedantic -O3

%.o, %.c:
	${CC} ${object} $@ $<

%.so: aes.o
	${CC} ${shared} aes.so aes.o

test: aes.so
	python3 -m unittest -v

demo: aes.so
	./hid_listen.mac > hid_pipe & python3 demo.py < hid_pipe

screen: screens.o aes.o
	${CC} screens.o aes.o -o ${flags} screen

clean:
	rm -f *.so *.o *.out screen
	rm -R __pycache__
	rm -f hid.PID
