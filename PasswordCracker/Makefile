GXX = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-error=unused-variable -c


all: main

main: main.o producer.o consumer.o
	gcc main.o producer.o consumer.o sha-256.c -lpthread -o main

main.o: main.c 
	gcc $(CFLAGS) main.c

producer.o: producer.c
	gcc $(CFLAGS) producer.c

consumer.o: consumer.c
	gcc $(CFLAGS) consumer.c
 
clean:
	rm main *.o
