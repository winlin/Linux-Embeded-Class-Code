#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	int try_open_count = 0;

	while( 1 )
	{
		fd = open( "/dev/firstchardevdriver0", O_RDWR );
	
		if( 0 < fd ) {
			break;
		}
		printf("Cann't open device file: /dev/firstchardevdriver0 !\n");
		printf("Trying next: %d !\n", ++try_open_count);
		sleep( 2 );
	}
	printf("Success to open device: /dev/firstchardevdriver0 !\n");

	printf("Press any key to exit...\n");
	getchar();

	close( fd );
	printf("Success to test device: /dev/firstchardevdriver[0] ! \n");

	return 0;
}

