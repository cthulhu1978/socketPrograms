server: server.c
	cc -c -g server.c
	cc -o server server.o

client: client.c
	cc -c -g client.c
	cc -o client client.o

clean:
	rm -f server
	rm -f client
	rm -f *.o 
