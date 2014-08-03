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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strcpy
#include <netdb.h>     // gethostbyname
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	// Check if there was any hostname specified on cmd-line
	if(argc < 2) 
	{
		fprintf(stderr, "%s\n", "No hostname specified!");
		return EXIT_FAILURE;
	}

	// First argument is hostname to look for
	const char* arg_host = argv[1];

	// Try to resolve hostname specified on cmd-line
	struct hostent* h = gethostbyname(arg_host);
	if(0 == h)
	{
		fprintf(stderr, "Unable to resolve host '%s'\n", arg_host);
		return EXIT_FAILURE;
	}

	// Check if there are any aliases and if so print header - Alias(es)
	if(h->h_aliases[0] != 0) 
	{
		fprintf(stdout, "Alias(es)\n");
	}

	// Print all aliases
	int i = 0;
	for(char* alias = h->h_aliases[i]; alias != NULL; i++)
	{
		fprintf(stdout, "%s\n", alias);
	}

	// Check if there are any IPs and if so print header - IP(s)
    if(h->h_addr_list[0] != 0)
    {
		fprintf(stdout, "IP(s)\n");
	}

	// Allocate space for IP converted to string
	char ip[INET6_ADDRSTRLEN + 1];
    ip[INET6_ADDRSTRLEN] = '\0';

	// Print all IPs
    struct in_addr** addr_list = (struct in_addr **) h->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        fprintf(stdout, "%s\n", ip);
    }

    // End with success error code - 0
	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
