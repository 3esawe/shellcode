#include <pcap.h>
#include "net-lib.h"


void pcap_fatal(const char *failed_in, const char *error){
	printf("Fatal error in %s: %s\n", failed_in, error);
	exit(1);
}


int main()
{
	/* code */
	struct pcap_pkthdr pcap_header; // the struct pcap_pkthdr contains extra information about the packets 
	const u_char *packet;
	char errbuf[PCAP_ERRBUF_SIZE]; // PCAP_ERRBUF_SIZE is defined in pcap.h as 256 bytes 
	char *device; // the device we want to sniff on 
	pcap_t *pcap_handle; // is same pointer as socket pointer but is used to refrence packet-captuaring object
	int i; 

	device = pcap_lookupdev(errbuf); // returns a pointer to a network device suitable for use with pcap_open_live() or looks for device to sniff on 
	if (device == NULL){
		pcap_fatal("pcap_lookupdev ", errbuf);
	}

	printf("sniffing on %s ", device);

	pcap_handle = pcap_open_live(device, 4096, 1, 0 , errbuf); /* is similar to the socket function and it opens a packet-capturing device, returning handle to it
	the argumet are the device , the max packet size a promiscuous flag, and timeout flag and pointer to error buffer 

*/ 

	for (int i = 0; i < 10; ++i)
	{

		packet = pcap_next(pcap_handle, &pcap_header); /* this function will grap the next packet it's passed the result from pcap_open_live  and pointer to pcap_pkthdr struct
		it returns a pointer to the packet*/
		printf("Got %d bytes \n",pcap_header.len);
		dump(packet, pcap_header.len);
		/* code */
	}
	pcap_close(pcap_handle);
	return 0;
}