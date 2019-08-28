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
#include "net-lib.h"


int main(void)
{
	int i, recv_length, sockfd;

	u_char buffer[9000];
	if ((sockfd = socket (PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1){ // we need root privilage because we are accessing kernal stuff 
		exit(1);
	}
	for (int i = 0; i < 3; ++i)
	{
		/* code */
		recv_length = recv(sockfd, buffer, 9000, 0);
		printf("Got a %d bytes packet\n", recv_length);
		dump(buffer , recv_length);
	}
	return 0;
}