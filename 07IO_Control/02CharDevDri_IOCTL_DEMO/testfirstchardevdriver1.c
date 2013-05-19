#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctlrealled.h"

int main(int argc, char **argv)
{
	int fd;
	char ch;
	
	fd=open("/dev/firstchardevdriver1",O_RDWR);
	if(0>fd){
		fd=open("/dev/firstchardevdriver1",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device :/dev/firstchardevdriver[1] !\n");
		exit(-1);
	}
	printf("Success to open device :/dev/firstchardevdriver[1] !\n");
	
	while(1){
		printf("Starting to control the LED1 on Real6410...\n");
		printf("\t1. Press key \'n\' to light on LED1 \n");
		printf("\t2. Press key \'f\' to light off LED1 \n");
		printf("\t3. Press key \'q\' to quit... \n");
		
		ch=getchar();
		getchar();
		if('q'==ch){
			break;
		}
		switch(ch){
		case 'n':
			ioctl(fd, LED_IOCTL_ON, 0);
			printf("LED1 should light on \n");
			break;
		case 'f':
			ioctl(fd, LED_IOCTL_OFF, 0);
			printf("LED1 should light off \n");
			break;
		default:
			break;
		}
		sleep(1);
	}
	
	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[1] !\n");
	
	return 0;
}
