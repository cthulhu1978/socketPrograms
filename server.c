#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_CLIENTS 5
#define BUFFER_SIZE 255

void error(const char * msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {

  if(argc < 2){
    fprintf(stderr, "Port number not provided, program terminated\n");
    exit(1);
  }

  int sockfd, newsockfd, portno, n;
  char buffer[BUFFER_SIZE];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){ error("Error opening socket");}
  // use mset
  bzero( (char*)&serv_addr, sizeof(serv_addr) );
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    error("Binding failed");
  }

  listen(sockfd, MAX_CLIENTS );
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if(newsockfd < 0){
    error("errod on newsockfd");
  }

  while (1) {
    bzero(buffer, BUFFER_SIZE);
    n = read(newsockfd, buffer, BUFFER_SIZE);
    if(n < 0){error("error on reading");}
    printf("Client : %s\n",buffer );
    bzero(buffer, BUFFER_SIZE);
    fgets(buffer, BUFFER_SIZE, stdin);

    n = write(newsockfd, buffer, strlen(buffer));

    if(n < 0){error("error on writing");}
    int i = strncmp("Bye", buffer, 3);
    if(i == 0){
       break;
    }
  }
    close(sockfd);
    close(newsockfd);
  return 0;
}
