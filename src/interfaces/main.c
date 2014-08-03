
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
#include <ifaddrs.h>
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

// See http://man7.org/linux/man-pages/man3/getifaddrs.3.html

int main(int argc, char** argv)
{
	struct ifaddrs *ifaddr = 0;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		fprintf(stderr, "Unable to get info about interfaces, reason: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	/* Walk through linked list, maintaining head pointer so we
	can free list later */

	int n = 0;
	struct ifaddrs *ifa = 0;
	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
	{
		if (ifa->ifa_addr == NULL)
		{
			continue;
		}

		int family = ifa->ifa_addr->sa_family;

		// Display interface name and family (including symbolic form of the latter for the common families)

		printf("%-8s %s (%d)\n",
			ifa->ifa_name,
			(family == AF_INET) ? "AF_INET" :
			(family == AF_INET6) ? "AF_INET6" : "???",
			family);

		if (family == AF_INET || family == AF_INET6)
		{
			int s = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			
			if (s != 0)
			{
				printf("getnameinfo() failed: %s\n", strerror(errno));
				return EXIT_FAILURE;
			}

			printf("\t\taddress: <%s>\n", host);

		}
	}
	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
