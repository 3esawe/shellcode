#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "net-lib.h"
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BACKLOG 4
#define PORT "443"

int main(int argc, char const *argv[])
{
	int sockfd, newsock;
	struct addrinfo hints, *p, *services;
	int status, numbytes;
	unsigned char buff[4096];
	struct sockaddr_storage conn_addr;
	socklen_t their_size;
	if (argc < 2){
		printf("Enter the link\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(argv[1], PORT, &hints, &services)) != 0){
		printf("getaddrinfo error\n");
		exit(2);
	}


	for(p=services; p != NULL ; p=p->ai_next){
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			printf("Error in socket\n");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			printf("Error in connect\n");
			continue;
		}
		break;		
	}
	freeaddrinfo(services);

	if (p == NULL){
		printf("Error in connecting\n");
		exit(3);
	}
		if(listen(sockfd, BACKLOG) == -1){
		printf("failed on listining\n");
		exit(2);
	}
	while(1){
		their_size = sizeof conn_addr;
		newsock = accept(sockfd, (struct  sockaddr *) &conn_addr, &their_size);
		if (newsock == -1){
			printf("Error in accept\n");
			continue;
		}
		//printf("server has connection from %s with PORT: %s\n",inet_ntop(conn_addr.ss_family, get_addr_in((struct sockaddr *) &conn_addr),s ,sizeof s), PORT);
		if(send(newsock, "HEAD / HTTP/2.0\r\n\r\n", 16, 0) == -1){
			printf("error in sending\n");
			perror("send");
		}
	if ((numbytes = recv(newsock, &buff, 512-1,0)) == -1){
		perror("recv");
		exit(1);
		}
		while(numbytes > 0){
			if(strncasecmp(buff, "Server:", 7) == 0){
				printf("this webiste %s server id is %s\n",argv[1], buff );
				exit(1);
			}
			else{
				printf("Error in server\n");
				exit(2);
			}
				// buff[numbytes] = '\0';
				// dump(buff, 512);
				// numbytes =  recv(newsock, &buff, 512-1,0);
		}
		close(newsock);
	}

	
	return 0;
}