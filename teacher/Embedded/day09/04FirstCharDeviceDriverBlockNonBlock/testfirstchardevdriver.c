#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	int fdnb;
	char buf[ 10 ];
	char ch;

	fd = open( "/dev/firstchardevdriver", O_RDWR );
	if( 0 > fd ) {
		fd = open( "/dev/firstchardevdriver0", O_RDWR );
	}
	if( 0 > fd ) {
		printf("Failure to open device file: /dev/firstchardevdriver[0] !\n");
		exit( -1 );
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] !\n");

	fdnb = open( "/dev/firstchardevdriver", O_RDWR | O_NONBLOCK );
	if( 0 > fd ) {
		fd = open( "/dev/firstchardevdriver0", O_RDWR | O_NONBLOCK );
	}
	if( 0 > fd ) {
		printf("Failure to open device file: /dev/firstchardevdriver[0] for non-block operation! \n");
		exit( -1 );
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] for non-block operation !\n");

	while( 1 ) {
		printf("Starting to test /dev/firstchardevdriver0...\n");
		printf("\t1. Press key \'r\' to read \n");
		printf("\t2. Press key \'b\' to no-block read \n");
		printf("\t3. Press key \'w\' to write \n");
		printf("\t4. Press key \'q\' to exit... \n");

		ch = getchar();
		getchar();

		if( 'q' == ch ) {
			break;
		}

		switch( ch ) {
		case 'r':
			printf("test before read !\n");
			read( fd, buf, 0 );
			printf("test after read !\n");
			break;
		case 'b':
			printf("test before non-block read !\n");
			read( fdnb, buf, 0 );
			printf("test after non-block read !\n");
			break;
		case 'w':
			printf("test before write !\n");
			write( fd, buf, 0 );
			printf("test after write !\n");
			break;
		default:
			break;
		}

		sleep( 1 );
	}

	close( fd );
	close( fdnb );
	printf("Success to test device: /dev/firstchardevdriver[0] ! \n");

	return 0;
}

