#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	char ch;
	int sdat;
	char sbuf[ 50 ] = "Test data from user space to kernel space !";
	char rbuf[ 50 ] = { 0 };

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
		printf("Starting to test /dev/firstchardevdriver0...\n");
		printf("\t1. Press key \'r\' to read string from kernel \n");
		printf("\t2. Press key \'s\' to read integer from kernel \n");
		printf("\t3. Press key \'w\' to write string to kernel \n");
		printf("\t4. Press key \'x\' to write integer 0x55aa55aa to kernel \n");
		printf("\t5. Press key \'y\' to write integer 0xaa55aa55 to kernel \n");
		printf("\t6. Press key \'q\' to quit... \n");

		ch = getchar();
		getchar();

		if( 'q' == ch ) {
			break;
		}

		switch( ch ) {
		case 'r':
			read( fd, rbuf, 50 );
			printf("User space read string from kernel: %d !\n", rbuf);
			break;
		case 's':
			read( fd, &sdat, 4 );
			printf("User space read integer from kernel: 0x%8x !\n", sdat);
			break;
		case 'w':
			write( fd, sbuf, 50 );
			printf("User space write string to kernel: %s !\n", sbuf);
			break;
		case 'x':
			sdat = 0x55aa55aa;
			write( fd, &sdat, 4 );
			printf("User space write integer to kernel: 0x%8x !\n", sdat);
			break;
		case 'y':
			sdat = 0xaa55aa55;
			write( fd, &sdat, 4 );
			printf("User space write integer to kernel: 0x%8x !\n", sdat);
			break;
		deault:
			break;
		}

		sleep( 1 );
	}

	close( fd );
	printf("Success to test device: /dev/firstchardevdriver[0] ! \n");

	return 0;
}

