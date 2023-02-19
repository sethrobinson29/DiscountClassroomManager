#include "header_stack.h"
#include "client_interface.cpp"

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  //exit if no ip address is given
  if (argc != 2){
    fprintf(stderr, "usage: tcpcli <IPaddress>");
    exit(1);
  }
  //setup socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  connect(sockfd, (SA *) & servaddr, sizeof(servaddr));

  //run program
  client_interface(stdin, sockfd);

  exit(0);
}