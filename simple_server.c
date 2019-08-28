#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "net-lib.h"
#define PORT "1234"
#define BACKLOG 5
void *get_addr_in(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in *)sa)->sin_addr );
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr );
}


int main(int argc, char const *argv[])
{
	printf("starting\n");
	struct sigaction sa;
	struct addrinfo hints, *p, *serviceInfo;
	int sockfd, newsock, status, yes = 1, numbytes;
	struct sockaddr_storage conn_addr;
	socklen_t their_size;
	char s[INET6_ADDRSTRLEN];
	char buff[512];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	printf("starting2\n");

	if((status = getaddrinfo(NULL, PORT, &hints, &serviceInfo)) != 0){
		printf("Error in getaddinfo\n");\
		exit(1);
	}
	printf("starting3\n");

	for(p = serviceInfo; p != NULL; p = p->ai_next){
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			printf("Error in socket\n");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen)  == -1){
			printf("Error in binding\n");
			continue;
		}

		break;
	}
	printf("starting4\n");

	freeaddrinfo(serviceInfo);
	if (p == NULL){
		printf("failed in estabilishing connection\n");
		exit(3);
	}

	if(listen(sockfd, BACKLOG) == -1){
		printf("failed on listining\n");
		exit(2);
	}
	printf("the server is waiting for connection\n");
	while(1){
		their_size = sizeof conn_addr;
		newsock = accept(sockfd, (struct  sockaddr *) &conn_addr, &their_size);
		if (newsock == -1){
			printf("Error in accept\n");
			continue;
		}
		printf("server has connection from %s with PORT: %s\n",inet_ntop(conn_addr.ss_family, get_addr_in((struct sockaddr *) &conn_addr),s ,sizeof s), PORT);
		if(send(newsock, "my name is omar\n", 30, 0) == -1){
			printf("error in sending\n");
			perror("send");
		}
	if ((numbytes = recv(newsock, &buff, 512-1,0)) == -1){
		perror("recv");
		exit(1);
		}
		while(numbytes > 0){
				buff[numbytes] = '\0';
				dump(buff, 512);
				numbytes =  recv(newsock, &buff, 512-1,0);
		}
		close(newsock);
	}

	// buff[numbytes] = '\0';
	// printf("%s\n",buff );
	return 0;
}