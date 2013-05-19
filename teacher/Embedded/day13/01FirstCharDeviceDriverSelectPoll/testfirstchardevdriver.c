#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	char buf[ 10 ];
	char ch;
	fd_set fdsr;
	struct timeval tv;
	int ret;

	fd = open( "/dev/firstchardevdriver", O_RDWR );
	if( 0 > fd ) {
		fd = open( "/dev/firstchardevdriver0", O_RDWR );
	}
	if( 0 > fd ) {
		printf("Failure to open device file: /dev/firstchardevdriver[0] !\n");
		exit( -1 );
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] !\n");

	while( 1 ) {
		ret = 0;
		FD_ZERO( &fdsr );
		FD_SET( fd, &fdsr );
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		ret = select( fd + 1, &fdsr, NULL, NULL, &tv );

		if( 0 > ret ) {
			printf("Select error: 0x%lx !\n", ret);
		}
		else if( 0 == ret ) {
			printf("Select timeout !\n");
		}
		else {
			if( FD_ISSET( fd, &fdsr ) ) {
				printf("Device is ready for read !\n");
			}
			else {
				printf("Device is not ready for read !\n");
			}
		}

		printf("Starting to test /dev/firstchardevdriver0...\n");
		printf("\t1. Press key \'r\' to read \n");
		printf("\t2. Press key \'w\' to write \n");
		printf("\t3. Press key \'q\' to exit... \n");

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
	printf("Success to test device: /dev/firstchardevdriver[0] ! \n");

	return 0;
}

