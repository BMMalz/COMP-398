GXX = gcc
CFLAGS =  -g -Wall -Wvla -Werror -Wno-error=unused-variable


all: main

main: main.c
	$(GXX) main.c -o main $(CFLAGS) -lpthread
	chmod 755 main
clean:
	rm main *.o
