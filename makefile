server: server.c
	cc -c -g server.c
	cc -o server server.o

clean:
	rm -f server
	rm -f *.o 
