/*
  这个目录用于演示读取设备文件
  和用户空间的信息的交换
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	
	fd=open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd=open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device :/dev/firstchardevdriver[0] !\n");
		exit(-1);
	}
	printf("Success to open device :/dev/firstchardevdriver[0] !\n");
	
	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	
	return 0;
}
