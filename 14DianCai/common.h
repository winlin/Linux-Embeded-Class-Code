#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LINK_TEST   0
#define MENU_SEND   1

struct menu_head {
	unsigned char type;
	int deal[];
}__attribute__((packed));


void show_menu(void)
{
	int fd;
	unsigned long offset = 0;
	fd = open("menulist.txt",O_RDONLY);
	if (0 > fd) {
		printf("Can't open menulist.txt file!\n");
		exit(-1);
	}

	char menu_ten[10][100]={0};
	
	while (1) {
		if (0 > lseek(fd, offset, SEEK_SET)) {
			perror("show_menu::lseek()");
			exit(-1);
		}
		if (read(fd,menu_ten,sizeof(menu_ten)) != sizeof(menu_ten)) {
			perror("show_menu::read");
			exit(-1);
		}
		
		printf("==============================================\n");
		printf("   *** Welcome To WinLin Restaurant ***\n");
		printf("==============================================\n");
		int i;
		for (i=0; i<10; ++i)
			printf("%s\n",menu_ten[i]);
		printf("==============================================\n");
		printf("Next Page(+)     End(q)      Previous Page(-)\n");

		int choose;
		while(1)
		{
			scanf("%d",&choose);
			
			if((ret >= '1') && (ret <= '8'))
			{
				break;
			}
		}
	}
	
	return ret;
}


void add_choose(int choose)
{
	
}
#endif
