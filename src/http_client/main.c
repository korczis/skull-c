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
#include <unistd.h>    // close
#include <netdb.h>     // gethostbyname
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	const char* prefix = "http://";
	const char* url = argc > 1 ? argv[1] : "http://google.com";

	if(strstr(url, prefix) == 0)
	{
		fprintf(stderr, "Invalid URL '%s', must start with '%s'\n", url, prefix);
		return EXIT_FAILURE;
	}

	url = url + strlen(prefix);

	const char* path = strstr(url, "/");
	int hostname_len = 0;
	if(!path)
	{
		hostname_len = strlen(url);
		path = "/";
	} 
	else
	{
		hostname_len = path - url;
	}

	char hostname[hostname_len + 1];
	hostname[hostname_len] = '\0';

	strncpy(hostname, url, hostname_len);

	// Print information about host and path
	// fprintf(stdout, "HOST: %s\n", hostname);
	// fprintf(stdout, "PATH %s\n", path);

	// Try to resolve name
	struct hostent* h = gethostbyname(hostname);
	if(0 == h || h->h_addr_list[0] == 0)
	{
		fprintf(stderr, "Unable to resolve host '%s'\n", hostname);
		return EXIT_FAILURE;
	}

	// Try to open socket
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 == s)
	{
		fprintf(stderr, "Unable to open socket, reason: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = *(in_addr_t*)h->h_addr_list[0];
	servaddr.sin_port = htons(80);

	int res = connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(0 != res)
	{
		fprintf(stderr, "Unable to connect to host, reason: %s\n", strerror(errno));
		close(s);
		return EXIT_FAILURE;
	}

	// Dynamically allocate memory
	size_t buffer_size = 256 * 1024;
	char* buffer = (char*) malloc(buffer_size);

	// Zero buffer memory area
	memset(buffer, 0, buffer_size);

	// Construct payload
	const int real_size = snprintf(buffer, buffer_size, "GET %s HTTP/1.0\n\n", path);
	// fprintf(stdout, "PAYLOAD: %s\n", buffer);

	res = sendto(s, buffer, real_size, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(res != real_size) 
	{
		fprintf(stderr, "Unable to send request, reason: %s\n", strerror(errno));
		
		// Free allocated memory
		free(buffer);
		buffer = 0;

		// Close socket
		close(s);
		s = 0;

		return EXIT_FAILURE;
	}

	// Print RESPONSE to console
	// fprintf(stdout, "RESPONSE:\n");
	
	// Process received bytes
	int received_bytes = 0;
	do
    {
		received_bytes = recvfrom(s, buffer, buffer_size - 1, 0, NULL, NULL);
		if(received_bytes > 0)
		{
			buffer[received_bytes] = '\0';
			fprintf(stdout, "%s", buffer);	
		}
	} while (received_bytes > 0);

	// Add trailing newline
	fprintf(stdout, "\n");

	// Free allocated memory
	free(buffer);
	buffer = 0;

	// Close socket
	close(s);
	s = 0;

	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
