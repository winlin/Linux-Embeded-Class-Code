#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>


int main(int argc,char **argv)
{
	int fd;
	char buf[10];

	fd = open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd = open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device file: /dev/firstchardevdriver[0]!\n");
		exit(-1);
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] !\n");

	read(fd,buf,0);
	write(fd,buf,0);
	ioctl(fd,0,0);

	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	return 0;
}
