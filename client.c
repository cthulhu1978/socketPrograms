/*
filename serv_addr portnumber
argv[0] = filename
argv[1] = serv_addr
argv[2] = portno
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 255

void error(const char* msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[BUFFER_SIZE];
  if(argc < 3){
    fprintf(stderr, "Usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {error("Error opening socket");}

  server = gethostbyname(argv[1]);
  if(server == NULL){
    fprintf(stderr, "ERROR, no such host\n");
  }
  bzero( (char*) &serv_addr, sizeof(serv_addr) );
  serv_addr.sin_family = AF_INET;
  bcopy( (char*)server->h_addr , (char*)&serv_addr.sin_addr.s_addr, server->h_length  );
  serv_addr.sin_port = htons(portno);
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 ){
    error("connection failed");
  }

  while (1) {
    bzero(buffer, BUFFER_SIZE);
    fgets(buffer, BUFFER_SIZE, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0){error("Error on writing ");}
    bzero(buffer, BUFFER_SIZE);
    n = read(sockfd, buffer, BUFFER_SIZE);
    if(n < 0){error("Error on read");}
    printf("Server: %s\n", buffer );

    int i = strncmp("Bye", buffer, 3);
    if(i == 0) {break; }
  }

  close(sockfd);






  return 0;
}
