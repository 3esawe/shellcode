#include <pcap.h>
#include "net-lib.h"
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

void pcap_fatal(const char *, const char *);
void decode_ethernet(const u_char *);
void decode_ip(const u_char *);
u_int decode_tcp(const u_char *);

void callback(u_char *, const struct pcap_pkthdr *, const u_char *); // this the prototype for the callback function which called by pcap_loop
// when there's a captured packet 


int main()
{
	/* code */

	struct pcap_pkthdr cap_headr;
	const u_char *packet, *pkt_data;
	char errbuffer[PCAP_ERRBUF_SIZE];
	char *device;
	pcap_t *handle;
	device = pcap_lookupdev(errbuffer);
	
	if (device == NULL){
		pcap_fatal("pcap_lookupdev" ,errbuffer);
	}
	printf("sniffing on device %s\n", device);

	handle = pcap_open_live(device , 5000, 1, 0, errbuffer);
	if ( handle == NULL){
		pcap_fatal("pcap_open_live", errbuffer);
	}
	/*pcap_loop is a better function than pcap_next it uses callback function as third argument this means pcpa_loop function is passed a function pointer which is called each time 
	packet is captured */
	pcap_loop(handle, 10, callback, NULL);
	pcap_close(handle);
	return 0;
}

void callback(u_char *user_args, const struct pcap_pkthdr *cap_headr, const u_char *packet){
	int tcphdr_len, tothdr_size, pkt_len;
	 u_char *pkt_data;

	 printf("#####Got a %d bytes#####\n", cap_headr->len);

	 decode_ethernet(packet);
	 decode_ip(packet+ETHER_HDR_LEN);
	 tcphdr_len = decode_tcp(packet + ETHER_HDR_LEN + sizeof (struct ip_hdr));

	 tothdr_size = ETHER_HDR_LEN+sizeof (struct ip_hdr)+ tcphdr_len;
	 pkt_data = (u_char *) packet + tothdr_size;
	 pkt_len = cap_headr->len - tothdr_size;

	 if (pkt_len > 0){
	 	printf("\t\t\t%u bytes of packets data\n", pkt_len);
	 	dump(packet, pkt_len);
	 }
	 else{
	 	printf("\t\t\tNO packet\n");
	 }

}

void pcap_fatal(const char * failed, const char * err){
	printf("Fatal in %s with error of %s\n",failed, err );
	exit(1);
}

void decode_ethernet(const u_char *start){
	int i;
	const struct ether_hdr *eth_hdr;

	eth_hdr = (struct ether_hdr *) start;
	printf("[[  Layer 2 :: Ethernet Header  ]]\n");
	printf("[ Source: %02x", eth_hdr->ether_src_addr[0]);
	for (int i = 1; i < ETHER_ADDR_LEN; ++i)
	{
		/* code */
		printf(":%02x", eth_hdr->ether_src_addr[i]);


	}

	printf("\tDest: %02x", eth_hdr->ether_dest_addr[0]);
	for(i=1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", eth_hdr->ether_dest_addr[i]);
	printf("\tType: %hu ]\n", eth_hdr->ether_type);
}


void decode_ip(const u_char *header_start) {
	const struct ip_hdr *ip_header;
	ip_header = (const struct ip_hdr *)header_start;
	struct sockaddr_in sa;
	char str_src[INET_ADDRSTRLEN];
	char str_dest[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(ip_header->ip_src_addr), str_src, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(ip_header->ip_dest_addr), str_dest, INET_ADDRSTRLEN);
	printf("\t((  Layer 3 ::: IP Header  ))\n");
	printf("\t( Source: %s\t", str_src);
	printf("Dest: %s )\n", str_dest);
	printf("\t( Type: %u\t", (u_int) ip_header->ip_type);
	printf("ID: %hu\tLength: %hu )\n", ntohs(ip_header->ip_id), ntohs(ip_header->ip_len));
}

u_int decode_tcp(const u_char *header_start){
	u_int headr_size;
	const struct tcp_hdr *tcp_header;

	tcp_header = (struct tcp_hdr *) header_start;

	headr_size = 4 * tcp_header->tcp_offset;
	printf("\t\t{{  Layer 4 :::: TCP Header  }}\n");
	printf("\t\t{ Src Port: %hu\t", ntohs(tcp_header->tcp_src_port));
	printf("Dest Port: %hu }\n", ntohs(tcp_header->tcp_dest_port));
	printf("\t\t{ Seq #: %u\t", ntohl(tcp_header->tcp_seq));
	printf("Ack #: %u }\n", ntohl(tcp_header->tcp_ack));
	printf("\t\t{ Header Size: %u\tFlags: ", headr_size);
	if(tcp_header->tcp_flags & TCP_FIN)
		printf("FIN ");
	if(tcp_header->tcp_flags & TCP_SYN)
		printf("SYN ");
	if(tcp_header->tcp_flags & TCP_RST)
		printf("RST ");
	if(tcp_header->tcp_flags & TCP_PUSH)
		printf("PUSH ");
	if(tcp_header->tcp_flags & TCP_ACK)
		printf("ACK ");
	if(tcp_header->tcp_flags & TCP_URG)
		printf("URG ");
	printf(" }\n");

	return headr_size;
}
