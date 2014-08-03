#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		fprintf(stderr, "%s\n", "No hostname specified!");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
