#include "settings.h"
#include "server.h"

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include <sys/types.h> // fork
#include <unistd.h> // fork

#include <arpa/inet.h> // inet_ntop()

#include <signal.h>

struct sigaction oldact;
int sockfd=-1;
int pid=-1;

void sighandler(int sig) {
	printf("\nCleaning up and exiting...\n");
	if (sockfd != -1) {
		if (pid > 0) { // parent process
			printf("Closing socket fd %d (port %d)\n", sockfd, PORT);
			close(sockfd);
			sockfd = -1;
		}
	}
	printf("\n");
	exit(0);
}

void setup_signals() {
	struct sigaction act;
	act.sa_handler = sighandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, &oldact);
	sigaction(SIGHUP, &act, &oldact);
	sigaction(SIGTERM, &act, &oldact);
}

int main( int argc, char *argv[] ) {
	int newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	setup_signals();
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int flag=1;
	if (-1 == setsockopt(sockfd, SOL_SOCKET,
				         SO_REUSEADDR, &flag, sizeof(flag))) {
		perror("setsockopt(...SO_REUSEADDR...) failure");
		exit(1);
	}
	
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	
	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = PORT;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1); }
	
	/* Now start listening for the clients, here
		* process will go in sleep mode and will wait
		* for the incoming connection
	*/
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	printf("Server started on port %d\n", PORT);
	
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		printf("Client connected\n");
		
		if (newsockfd < 0) {
			perror("ERROR on accept");
			exit(1);
		}
		
		/* Create child process */
		pid = fork();
		
		if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}
		
		if (pid == 0) {
			/* This is the client process */
			close(sockfd);
			doprocessing(newsockfd, &cli_addr);
			exit(0);
		}
		else {
			close(newsockfd);
		}
		
	} /* end of while */
}

void doprocessing(int sock, struct sockaddr_in *cli_addr) {
	int n;
	char buffer[MAXBUF+1];
	struct addrinfo *p;
	char ipstr[INET6_ADDRSTRLEN];
	/* const char *inet_ntop(int af, const void *src, */
	/* 					char *dst, socklen_t size); */
	printf("doprocessing()\n"); fflush(stdout);
	if (!inet_ntop(AF_INET,
			get_in_addr((struct sockaddr *)cli_addr),
			ipstr, sizeof ipstr)) {
		perror("inet_ntop() returned NULL");
		exit(0);
	}
	printf("Client connected on %s\n", ipstr); fflush(stdout);
	while (1) {
		bzero(buffer,MAXBUF);
		n = read(sock,buffer,MAXBUF);
		
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		if (n == 0) {
			printf("Client disconnected (EOF on socket)\n");
			exit(0);
		}
		
		printf("Received from %s: %d bytes: {%*s}\n",
			ipstr, n, n, buffer);
		/* n = write(sock,"I got your message", 18); */
		/* if (n < 0) { */
		/* 	perror("ERROR writing to socket"); */
		/* 	exit(1); */
		/* } */
	}
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

