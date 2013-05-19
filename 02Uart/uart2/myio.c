#define UTRSTAT0 0x7F005010
#define UTX0     0x7f005020
#define URX0     0x7F005024

void put_c(char c)
{
	volatile unsigned int *stat=(unsigned int *)UTRSTAT0;
	while(!((*stat) & 0x2));
	volatile unsigned int *utx=(unsigned int *)UTX0;
	*utx=c; 
	
	if(c=='\n')
		put_c('\r');
}

void put_s(char *string)
{
	int i=0;
	while(string[i]!='\0'){
		put_c(string[i]);
		++i;
	}
}

char get_c(void)
{
	volatile unsigned int *stat=(unsigned int *)UTRSTAT0;
	while(!((*stat) & 0x1));
	volatile unsigned int *urx=(unsigned int *)URX0;
	return (char)*urx;
}

void get_s(char* s,int size)
{
	int i=0;
	while(i<size-1){
		char cc=get_c();
		if(cc!='\r'){
			s[i++]=cc;
			put_c(cc);
		}
		else
			break;
	}
	s[i]='\0';
	put_c('\n');
}

int str_equl(char* a,char* b)
{
	while(*a!='\0'&&*b!='\0'){
		if(*a==*b){
			++a;++b;
		}
		else
			return 1;
	}
	
	if(*a=='\0'&&*b=='\0')
		return 0;
	else
		return 1;
}
/* convert a char number to a string in 16 mode*/
void ctoa_hex(char num,char *str)
{
	int i;
	for(i=1;i>=0;--i){
		if((str[i]=num&0xf)<10)
			str[i]+='0';
		else
			str[i]=str[i]+'a'-10;
		num=num>>4;
	}
	str[2]='\0';
}
/* convert a int number to string in 16 mode */
void itoa_hex(int num,char *str)
{
	int i;
	str[0]='0';str[1]='x';
	for(i=9;i>1;--i){
		if((str[i]=num&0xf)<10)
			str[i]+='0';
		else
			str[i]=str[i]+'a'-10;
		num=num>>4;
	}
	str[10]='\0';
}

/* print a char in 16 mode */
void print_char_hex(char cc)
{
	char c_str[3];
	ctoa_hex(cc,c_str);
	put_s(c_str);
}
/* print a int in 16 mode with 0x */
void print_int_hex(int n)
{
	char str_len[12];
	itoa_hex(n,str_len);
	put_s(str_len);
}

/* convert a string to int */
int strtoint(const char* str)
{
	int n=0;
	while(*str){
		switch(*str){
		case '0':
			n=n*10+0;break;
		case '1':
			n=n*10+1;break;
		case '2':
			n=n*10+2;break;
		case '3':
			n=n*10+3;break;
		case '4':
			n=n*10+4;break;
		case '5':
			n=n*10+5;break;
		case '6':
			n=n*10+6;break;
		case '7':
			n=n*10+7;break;
		case '8':
			n=n*10+8;break;
		case '9':
			n=n*10+9;break;
		default:
			break;
		}
		++str;
	}
	return n;
}

/* append a string to other string */
char* append_string(char* str,const char* src,int len)
{
	char *p=NULL;
	while(*str)
		++str;
	p=str;
	while(*src){
		*p++=*src++;
	}
	*p=0;	
}
/* get a string with assigned length */
char* get_string(char* str,const char* src,int len)
{
	while((*src)!='\0'&&len--){
		*str++=*src;
	}
	return str;
}
/* convert a int number into string in dec-mode */
char* get_number_int(char* str,int src ,char flag,char pricision)
{
	if(flag==1)
		*str++='-';
	while(1){
		int tmp=src%10;
		src=src/10;
		*str++='0'+tmp;
		if(src==0)
			break;
	}
}

/* printf realize */
typedef char* va_list;
#define _INTSIZEOF(n)   ((sizeof(n)+sizeof(int)-1)&~(sizeof(int)-1))
#define va_start(ap,v)  (ap=(char*)&v+_INTSIZEOF(v))
#define va_arg(ap,t)    (*(t*)((ap+=_INTSIZEOF(t))-_INTSIZEOF(t)))
#define va_end(ap)      (ap=(va_list)0)

/* fromatted by the fm */
void my_vsprintf(char* printbuf,const char* fm, va_list args)
{
	char* str=printbuf;
	char precision[6];
	for(;*fm;++fm){
		if(*fm!='%'){
			*str++=*fm;
			continue;
		}
		++fm;
		switch(*fm){
		case '-':
			flag=1;
		case '+':
			flag=0;
		case '1':
			
		case 'c':
			*str++=va_arg(args,char);
		case 'd':
			str=get_number_int(str,va_arg(args,int),)
		}
		
	}
}

int my_printf(const char* fm,...)
{
	
}
