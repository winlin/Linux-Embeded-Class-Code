#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/mman.h>

#define GPL_CONO	0x810
#define GPL_CON1	0x814
#define GPL_DAT		0x818
#define GPL_PUD 	0x81C

#define GPN_CON		0x830
#define GPN_DAT		0x834
#define GPN_PUD		0x838

unsigned char *gpiobase;
unsigned long *gpl_con0;
unsigned long *gpl_con1;
unsigned long *gpl_dat;
unsigned long *gpl_pud;
unsigned long *gpn_con;
unsigned long *gpn_dat;
unsigned long *gpn_pud;

unsigned char led;

struct itimerval itv;

void sighandler_alarm(int signo)
{
	*gpl_con1 &= 0xFF0FFFFF;   // first clear the aim bits
	*gpl_con1 |= 0x00100000;   // set the aim bits
	*gpl_pud &= 0xF3FFFFFF;
	*gpn_con &= 0xFFFFCFFF;
	*gpn_con |= 0x00001000;
	*gpn_pud &= 0xFFFFCFFF;
	if(!led){
		led = 1;
		*gpl_dat |= 0x00002000;
		*gpn_dat &= 0xFFFFFFBF;
	}else{
		led = 0;
		*gpl_dat &= 0xFFFFDFFF;
		*gpn_dat |= 0x00000040;
	}
}

int main(int argc,char **argv)
{
	int fd;

	fd = open("/dev/firstchardevdriver",O_RDWR);
	if(0>fd){
		fd = open("/dev/firstchardevdriver0",O_RDWR);
	}
	if(0>fd){
		printf("Failure to open device file: /dev/firstchardevdriver[0]!\n");
		exit(-1);
	}
	printf("Success to open device file: /dev/firstchardevdriver[0] !\n");

	gpiobase = (unsigned char *)mmap(NULL,0x1000,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	gpl_con0 = (unsigned long *)(gpiobase + GPL_CONO);   // base address + offset
	gpl_con1 = (unsigned long *)(gpiobase + GPL_CON1);
	gpl_dat = (unsigned long *)(gpiobase + GPL_DAT);
	gpl_pud = (unsigned long *)(gpiobase + GPL_PUD);
	gpn_con = (unsigned long *)(gpiobase + GPN_CON);
	gpn_dat = (unsigned long *)(gpiobase + GPN_DAT);
	gpn_pud = (unsigned long *)(gpiobase + GPN_PUD);

	signal(SIGALRM,sighandler_alarm);
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 1;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&itv,NULL);

	led = 0;

	printf("Press any key to exit...");
	getchar();

	munmap(gpiobase,0x1000);

	close(fd);
	printf("Success to test device: /dev/firstchardevdriver[0] !\n");
	return 0;
}
