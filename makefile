all: server client

server: server.c common.h
	gcc -o server -lpthread server.c

client: client.c common.h
	gcc -o client client.c

clean:
	rm -rf client server
