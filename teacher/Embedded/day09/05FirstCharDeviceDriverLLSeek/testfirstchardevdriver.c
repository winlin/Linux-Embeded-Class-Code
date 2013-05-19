#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	char bufr[ 10 ];
	char bufw[ 10 ];
	char ch;
	int pos;
	int npos;
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

	for( pos = 0; pos < 10; pos++ ) {
		bufw[ pos ] = pos + 1;
	}

	pos = 0;
	while( 1 ) {
		printf("Starting to test /dev/firstchardevdriver...\n");
		printf("\t1. Press key \'l\' to seek to start \n");
		printf("\t2. Press key \'L\' to seek to end \n");
		printf("\t3. Press key \'s\' to seek to increase \n");
		printf("\t4. Press key \'S\' to seek to decrease \n");
		printf("\t5. Press key \'r\' to read 1 byte \n");
		printf("\t6. Press key \'R\' to read 2 bytes \n");
		printf("\t7. Press key \'w\' to write 1 byte \n");
		printf("\t8. Press key \'W\' to write 2 bytes \n");
		printf("\t9. Press key \'q\' to quit... \n");

		ch = getchar();
		getchar();

		if( 'q' == ch ) {
			break;
		}

		switch( ch ) {
		case 'l':
			npos = lseek( fd, 0 , SEEK_SET );
			printf("seek to position: %d from start \n", npos);
			break;
		case 'L':
			npos = lseek( fd, 0 , SEEK_END);
			printf("seek to position: %d from end \n", npos);
			break;
		case 's':
			npos = lseek( fd, 1 , SEEK_CUR );
			printf("seek to position: %d from current \n", npos);
			break;
		case 'S':
			npos = lseek( fd, -1 , SEEK_CUR );
			printf("seek to position: %d from current \n", npos);
			break;
		case 'r':
			ret = read( fd, bufr, 1 );
			printf("read %d bytes data: %d \n", ret, bufr[ 0 ]);
			break;
		case 'R':
			ret = read( fd, bufr, 2 );
			printf("read %d bytes data: %d  %d \n", ret, bufr[ 0 ], bufr[ 1 ]);
			break;
		case 'w':
			ret = write( fd, &bufw[ pos ], 1 );
			printf("write %d byte data: %d \n", ret, bufw[ pos ]);
			if( 1 == ret ) {
				printf("update pos increase \n");
				pos++;
			}
			break;
		case 'W':
			ret = write( fd, &bufw[ pos ], 2 );
			printf("write %d bytes data: %d  %d \n", ret, bufw[ pos ], bufw[ pos + 1 ]);
			if( 1 == ret ) {
				printf("update pos increase \n");
				pos++;
			}
			if( 2 == ret ) {
				printf("update pos plus 2 \n");
				pos += 2;
			}
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

