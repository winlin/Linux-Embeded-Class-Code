#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	char w_buf[50]="Data in User Space !";
	char r_buf[50]={0};
	
	fd=open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd=open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device :/dev/firstchardevdriver[0] !\n");
		exit(-1);
	}
	printf("Success to open device :/dev/firstchardevdriver[0] !\n");
	
	read(fd, r_buf, 50);
	printf("Read from Kernel Space string :%s\n",r_buf);
	write(fd,w_buf, 50);

	int w_int=0x11111111;
	int r_int=0;
	fd=open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd=open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device :/dev/firstchardevdriver[0] !\n");
		exit(-1);
	}
	printf("Success to open device :/dev/firstchardevdriver[0] !\n");
	
	read(fd, &r_int, 4);
	printf("Read from Kernel Space integer :0x%08x\n",r_int);
	write(fd,&w_int, 4);
	ioctl(fd,0,0);
	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	
	return 0;
}
