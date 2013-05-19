#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctlrealled.h"

int main(int argc, char **argv)
{
	int fd;
	int try_open_count=0;
	fd=open("/dev/firstchardevdriver1",O_RDWR);
	/*while(1){
		fd=open("/dev/firstchardevdriver1",O_RDWR);
		if(0<fd){
			break;
		}
		printf("Can't open device file :/dev/firstchardevdriver1");
		printf("Tring next :%d !\n",++try_open_count);
		sleep(2);
		}*/

	printf("Success to open device :/dev/firstchardevdriver1 !\n");
	printf("Press any key to continue...\n");
	getchar();
	
	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[1] !\n");
	
	return 0;
}
