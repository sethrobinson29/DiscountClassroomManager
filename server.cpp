#include "server_interface.cpp"


int main() {
    //Setup socket
  	int	listenfd, connfd, err;
	pid_t	childpid;
	socklen_t	clilen;
	struct sockaddr_in cliaddr, servaddr;

  	listenfd = socket(AF_INET, SOCK_STREAM, 0);

  	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

  	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);
	
	//loop forever
  	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		
    //Wait for connection
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR){
				std::cout << "REAPEAT loop\n";
				continue;		/* back to for() */
			}else {
				fprintf(stderr, "accept error");
				exit(1);
			}
    	}
		std::cout << "CONNECTED\n";
    	//Fork() when connection is made to continue listening
		if ( (childpid = fork()) == 0) {
			close(listenfd);										//close listening socket

			//run program
			server_interface(connfd);
			
			exit(0);
		}
		close(connfd);			//parent closes connected socket
  }

}