#include <stdio.h>

struct zerolengtharray {
	int cnt;
	unsigned char dat[0];
};

int idat[10];

int main(int argc, char **argv)
{
	int i;
	int count;
	struct zerolengtharray *zla;

	i = sizeof( struct zerolengtharray );
	printf("struct zerolengtharray size: %d !\n", i);

	count = 2;
	idat[0] = count * sizeof( int );
	for( i = 0; i < count; i++ )
	{
		idat[ i + 1 ] = 0x12345678;
	}

	zla = (struct zerolengtharray *)idat;
	for( i = 0; i < zla->cnt; i++ )
	{
		printf("zla->cnt[ %d ] = %2x \n", i, zla->dat[ i ]);
	}

	return 0;
}

