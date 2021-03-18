GXX = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable


all: main hasher

hasher: hasher.c
	$(GXX) hasher.c sha-256.c -o hasher $(CFLAGS) 
	chmod 755 hasher
main: main.c 
	$(GXX) main.c -o main $(CFLAGS) 
	chmod 755 main
clean:
	rm main *.o
