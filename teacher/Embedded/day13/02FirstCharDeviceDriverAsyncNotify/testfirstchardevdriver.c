#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <signal.h>

int fd;
char buf[ 10 ];

void chardevicedriver_signal_handler(int signum)
{
	printf("Received signal from device: /dev/firstchardevdriver0, signal: 0x%.4x !\n", signum);

	printf("Reading start... \n");
	read( fd, buf, 0 );
	printf("Reading exit... \n");
}

int main(int argc, char **argv)
{
	char ch;
	int oldflags;
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

	signal( SIGIO, chardevicedriver_signal_handler );
	fcntl( fd, F_SETOWN, getpid() );

	printf("Press any key to continue... \n");
	getchar();

	oldflags = fcntl( fd, F_GETFL );
	fcntl( fd, F_SETFL, oldflags | FASYNC );

	printf("Press any key to continue... \n");
	getchar();

	while( 1 ) {
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

