#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include <signal.h>

int fd;
char buf[10];

void chardevdriver_signal_handler(int signum)
{
	printf("Recieve signal from device : /dev/firstchardevdriver0, signal=0x%.4x \n",signum);

	printf("firstdevdriver_signal_handler():Reading start...\n");
	read(fd,buf,0);
	printf("firstdevdriver_signal_handler():Reading exit...\n");
}

int main(int argc,char **argv)
{
	char ch;
	int oldflags;
	fd_set fdsr;
	struct timeval tv;
	int ret;

	fd = open("/dev/firstchardevdriver",O_RDWR);
	if(fd<0){
		fd = open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device file: /dev/firstchardevdriver[0]!\n");
		exit(-1);
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] !\n");

	//register
	signal(SIGIO, chardevdriver_signal_handler);
	//set owner
	fcntl(fd, F_SETOWN, getpid());   
	printf("Press any key to continue 1...\n");
	getchar();

	oldflags=fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, oldflags|FASYNC);   // it will call Driver's fasync() function
	printf("Press any key to continue 2...\n");
	getchar();
	
	while(1){
		ret = 0;
		FD_ZERO(&fdsr);
		FD_SET(fd,&fdsr);
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		ret = select(fd + 1, &fdsr, NULL, NULL, &tv);

		if(0 > ret){
			printf("Select error: 0x%lx !\n");
		}else if(0 == ret){
			printf("Select timeout !\n");
		}else{
			if(FD_ISSET(fd,&fdsr)){
				printf("Device data is ready for read !\n");
			}else{
				printf("Device data is ready for read");
			}
		}

		printf("Starting to test /dev/firstchardevdriver0...\n");
		printf("\t1.Press key \'r\' to read \n");
		printf("\t2.Press key \'w\' to write \n");
		printf("\t3.Press key \'q\' to exit... \n");
		
		ch = getchar();
		getchar();

		if('q' == ch){
			break;
		}
		switch(ch){
		case 'r':
			printf("test before read !\n");
			read(fd,buf,0);
			printf("test after read !\n");
			break;
		case 'w':
			printf("test before write !\n");
			write(fd,buf,0);
			printf("test after write!\n");
			break;
		default:
			break;
		}
		sleep(1);
	}

	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	return 0;
}
