CFLAGS = -Wall -O2
LDFLAGS = -lpthread

all: server client

csapp.o: csapp.c
	gcc $(CFLAGS) -c csapp.c -o csapp.o

server: server.c csapp.o
	gcc $(CFLAGS) server.c csapp.o -o server $(LDFLAGS)

client: client.c csapp.o
	gcc $(CFLAGS) client.c csapp.o -o client $(LDFLAGS)

clean:
	rm -f *.o server client

run_server: server
	./server

run_client: client
	./client
