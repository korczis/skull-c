#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	const char* name = argc > 1 ? argv[1] : "Travis";

	fprintf(stdout, "Hello World %s!\n", name); // Declared in <stdio.h>
	
	return EXIT_SUCCESS;      // Declared in <stdlib.h>
}
