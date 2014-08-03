// Copyright, 2013-2014, by Tomas Korcak. <korczis@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included inso
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strcpy
#include <strings.h>
#include <unistd.h>    // close
#include <netdb.h>     // gethostbyname
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <net/ethernet.h>

static const unsigned MAX_MTU = 512;

// See http://www.binarytides.com/icmp-ping-flood-code-sockets-c-linux/
// See https://github.com/Justasic/Navn/blob/master/src/windows/iphdr.h
// See http://www.kernelthread.com/projects/hanoi/html/icmp.html
// See http://sock-raw.org/papers/sock_raw
// See http://austinmarton.wordpress.com/2011/09/14/sending-raw-ethernet-packets-from-a-specific-interface-in-c-on-linux/

/*
 * in_cksum --
 * Checksum routine for Internet Protocol
 * family headers (C Version)
 */
unsigned short cksum(unsigned short *addr, int len)
{
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)
    {
      sum += *w++;
      nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
    {
      *(u_char *) (&answer) = *(u_char *) w;
      sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);       /* add hi 16 to low 16 */
    sum += (sum >> 16);               /* add carry */
    answer = ~sum;              /* truncate to 16 bits */
    return (answer);
}

int main(int argc, char** argv)
{
	const char* hostname = argc > 1 ? argv[1] : "google.com";

	fprintf(stdout, "Sending ICMP request to '%s'\n", hostname); 

	struct hostent* h = gethostbyname(hostname);
	if(0 == h || h->h_addr_list[0] == 0)
	{
		fprintf(stderr, "Unable to resolve host '%s'\n", hostname);
		return EXIT_FAILURE;
	}

	int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
     
    if (s < 0) 
    {
        fprintf(stderr, "Unable to create socket, reason: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, "en3", IFNAMSIZ-1);

	ioctl(s, SIOCGIFADDR, &ifr);

	/* display result */
	fprintf(stdout, "%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    const int on = 1;
     
    // We shall provide IP headers
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on)) == -1) 
    {
    	fprintf(stderr, "Unable to setsockopt IPPROTO_IP IP_HDRINCL, reason: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
     
    // Allow socket to send datagrams to broadcast addresses
    if (setsockopt (s, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on)) == -1) 
    {
        fprintf(stderr, "Unable to setsockopt SOL_SOCKET SO_BROADCAST, reason: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }  

    int payload_size = 128;

    //Calculate total packet size
    int packet_size = sizeof (struct ip) + sizeof (struct icmp) + payload_size;
    char *packet = (char *) malloc (packet_size);

	//ip header
    struct ip *ip = (struct ip *) packet;
    struct icmp *icmp = (struct icmp *) (packet + sizeof (struct ip));
     
    //zero out the packet buffer
    memset (packet, 0, packet_size);
 
 	// See - http://www.kernelthread.com/projects/hanoi/src/hanoimania/hanoi-icmp.c

    ip->ip_v = 4;
    ip->ip_hl = 5;
    ip->ip_tos = 0;
    ip->ip_len = sizeof(struct ip) + sizeof(struct icmp); // htons (packet_size);
    ip->ip_id = rand ();
    ip->ip_off = 0;
    ip->ip_ttl = 255;
    ip->ip_p = IPPROTO_ICMP;
    ip->ip_src.s_addr = inet_addr("10.0.0.17"); // *(in_addr_t*)&((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
    ip->ip_dst.s_addr = *(in_addr_t*)h->h_addr_list[0];
    //ip->check = in_cksum ((u16 *) ip, sizeof (struct iphdr));
 
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_hun.ih_idseq.icd_seq = rand();
    icmp->icmp_hun.ih_idseq.icd_id = rand();
    //checksum
    icmp->icmp_cksum = 0;
     
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = *(in_addr_t*)h->h_addr_list[0]; // *(in_addr_t*)h->h_addr_list[0];
    memset(&servaddr.sin_zero, 0, sizeof (servaddr.sin_zero));

	struct sockaddr_in connection;
    socklen_t addrlen;
    char buffer[MAX_MTU];

    int sent = 0, sent_size = 0;
    while (1)
    {
    	// icmp->icmp_hun.ih_idseq.icd_seq = rand();
    	// icmp->icmp_hun.ih_idseq.icd_id = rand();

        memset(packet + sizeof(struct ip) + sizeof(struct icmp), rand() % 255, payload_size);
         
        //recalculate the icmp header checksum since we are filling the payload with random characters everytime
        icmp->icmp_cksum = 0;
        icmp->icmp_cksum = cksum((unsigned short *)icmp, sizeof(struct icmp));
        
        ip->ip_sum = cksum((unsigned short*)ip, sizeof(struct ip));
        if (sendto(s, packet, packet_size, 0, (struct sockaddr*) &servaddr, sizeof (servaddr)) != packet_size) 
        {
        	fprintf(stderr, "Unable to send packet, reason: %s\n", strerror(errno));
            break;
        }

        ++sent;
        printf("%d packets sent\r", sent);
        fflush(stdout);
         
        usleep(1e6);  //microseconds
    }

    free(packet);

    close(s);

	return EXIT_SUCCESS;
}
