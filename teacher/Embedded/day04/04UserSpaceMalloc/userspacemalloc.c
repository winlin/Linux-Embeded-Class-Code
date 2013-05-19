#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static char *dat = NULL;
static long allcnt = 30 * 1024 * 1024;

int main(int argc, char **argv)
{
	char ch;

	printf("Starting demostration:\n");
	printf("Press \'a\' key to allocate... \n");
	ch = getchar();
	getchar();
	if( 'a' != ch ) {
		printf("You press error key !\n");
		exit( -1 );
	}

	// allocate memory
	dat = malloc( allcnt );
	if(  NULL == dat )
	{
		printf("Failure to allocate memory size: %ld \n", allcnt);
		exit( -1 );
	}
	printf("Success to allocate memory size: %ld \n", allcnt);

	// using memory allocated
	printf("Press \'u\' key to use memory !\n");
	ch = getchar();
	getchar();
	if( 'u' != ch ) {
		printf("You press error key !\n");
		free( dat );
		exit( -1 );
	}
	printf("Using memory allocated... \n");
	memset( dat, 0, allcnt );

	// free memory and exit
	printf("Press any key to exit... \n");
	getchar();

	free( dat );
	dat = NULL;

	return 0;
}

