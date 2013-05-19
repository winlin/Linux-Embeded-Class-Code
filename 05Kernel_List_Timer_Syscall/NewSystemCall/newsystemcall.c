#include <stdio.h>
#include <linux/unistd.h>

int main(int argc, char** argv)
{
	int res;
	res=syscall(361, 100, 200);
	printf("The result is:%d\n",res);
	return 0;
}
