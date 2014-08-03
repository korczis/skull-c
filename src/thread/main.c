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

#include <pthread.h> // pthread_*

unsigned int factorial(unsigned int val)
{
	fprintf(stdout, "factorial(%u)\n", val);

	unsigned int res = 1;

	for(unsigned int i = 1; i <= val; i++)
	{
		res *= i;
	}

	return res;
}

void* thread_func(void* arg)
{
	fprintf(stdout, "thread_func(%p) - Start\n", arg);

	unsigned int val = arg ? *(unsigned int*)arg : 100;

	unsigned int res = factorial(val);

	fprintf(stdout, "thread_func(%p) -> %u\n", arg, res);
	return 0;
}

int main(int argc, char** argv)
{
	pthread_t thread = 0;

	unsigned int thread_arg = argc > 1 ? atoi(argv[1]) : 64;

	fprintf(stdout, "%s\n", "Creating thread");
	int res = pthread_create(&thread, 0, thread_func, &thread_arg);
	if(0 != res)
	{
		fprintf(stderr, "%s\n", "Unable to create thread.");
		return EXIT_FAILURE;
	}

	fprintf(stdout, "%s\n", "Waiting for thread");
	pthread_join(thread, 0);

	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
