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




int main(int argc, char const *argv[])
{
	struct addrinfo hints, *p, *service;
	char ipstr[INET6_ADDRSTRLEN];
	int status; 
	if (argc < 2){
		printf("Enter the host name\n");
	}

	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(argv[1],NULL,&hints,&service)) != 0){
		printf("Error in getaddrinfo\n");
		exit(0);
	}

	printf("The ip address for this host %s\n", argv[1]);

	for(p=service; p!=NULL; p = p->ai_next){
		char *name;
		void *addr;
		if (p->ai_family == AF_INET){
			name = "IPV4";
			struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
			addr = &(ipv4->sin_addr);
		}
		else{
			name = "IPV6";
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
			addr = &(ipv6->sin6_addr); 
		}

		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("ip addres of: %s is -> %s \n", argv[1],ipstr);
	}

	freeaddrinfo(service);
	return 0;
}