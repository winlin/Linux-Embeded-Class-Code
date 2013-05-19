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
	char w_buf[10];
	char r_buf[10];
	char ch;
	int pos;
	int npos;
	int ret;
	
	
	fd=open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd=open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device :/dev/firstchardevdriver[0] !\n");
		exit(-1);
	}
	printf("Success to open device :/dev/firstchardevdriver[0] !\n");
	
	for(pos=0;pos<10;++pos) {
		w_buf[pos]=pos+1;
	}
	pos=0;
	while(1) {
		printf("1.Press \'l\' to seek to start\n");
		printf("1.Press \'L\' to seek to end\n");
		printf("1.Press \'s\' to seek to increass \n");
		printf("1.Press \'S\' to seek to descress \n");
		printf("1.Press \'r\' to read 1 byte\n");
		printf("1.Press \'R\' to read 2 byte\n");
		printf("1.Press \'w\' to write 1 byte\n");
		printf("1.Press \'W\' to write 1 byte\n");
		printf("Press q to quit...\n");
		
		ch=getchar();
		getchar();
		
		if('q'==ch)
			break;
		switch(ch) {
		case 'l':
			npos=lseek(fd, 0, SEEK_SET);
			printf("Seek to position:%d from start \n",npos);
			break;
		case 'L':
			npos=lseek(fd, 0, SEEK_END);
			printf("Seek to position:%d from end \n",npos);
			break;
		case 's':
			npos=lseek(fd, 1, SEEK_CUR);
			printf("Seek to position:%d from current \n",npos);
			break;
		case 'S':
			npos=lseek(fd, -1, SEEK_CUR);
			printf("Seek to position:%d from current \n",npos);
			break;
		case 'r':
			ret=read(fd,r_buf, 1);
			printf("Read %d bytes data:%d\n", ret, r_buf[0]);
			break;
		case 'R':
			ret=read(fd, r_buf, 2);
			printf("Read %d bytes data:%d %d\n", ret, r_buf[0],r_buf[1]);
			break;
			
		case 'w':
			ret=write(fd, w_buf+pos, 1);
			printf("write %d byte data :%d\n", ret, w_buf[pos] );
			
			if(1==ret){
				//update pos
				pos++;
			}
			break;
				
		case 'W':
			ret=write(fd, w_buf+pos, 2);
			printf("write %d byte data :%d %d\n", ret, w_buf[pos], w_buf[pos] );
			if(1==ret){
				//update pos
				pos++;
			}
			if(2==ret){
				//update pos
				pos+=2;
			}
			break;
		default:
			break;
		}
	}

	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	
	return 0;
}
