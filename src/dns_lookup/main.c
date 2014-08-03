#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strcpy
#include <netdb.h>     // gethostbyname
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		fprintf(stderr, "%s\n", "No hostname specified!");
		return EXIT_FAILURE;
	}

	// First argument is hostname to look for
	const char* arg_host = argv[1];

	struct hostent *h = gethostbyname(arg_host);
	if(0 == h)
	{
		fprintf(stderr, "Unable to resolve host '%s'\n", arg_host);
		return EXIT_FAILURE;
	}

	struct in_addr **addr_list = (struct in_addr **) h->h_addr_list;
     
    char ip[INET6_ADDRSTRLEN + 1];
    ip[INET6_ADDRSTRLEN] = '\0';

    for(int i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        fprintf(stdout, "%s\n", ip);
        return 0;
    }

	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
