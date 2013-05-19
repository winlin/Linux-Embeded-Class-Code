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
void ctoa(char num,char *str)
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
void itoa(int num,char *str)
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
	ctoa(cc,c_str);
	put_s(c_str);
}
/* print a int in 16 mode with 0x */
void print_int_hex(int n)
{
	char str_len[12];
	itoa(n,str_len);
	put_s(str_len);
}

/*dm9000id:print vid,pid*/
void parse_cmd(char * cmd)
{
	if(!str_equl(cmd,"dm9000id")){
		unsigned int id=get_dm9000_id();
		char strbuf[12];
		itoa(id,strbuf);
		put_s(strbuf);
		put_c('\n');
	}
	else if(!str_equl(cmd,"arping")){
		
		send_arp();
		
		char recv_msg[1520];
		unsigned int len;
		len=dm9000_recv(recv_msg);
		if(len==0)
			put_s("Recieve no data!\n");
		else
			print_recv_arp(recv_msg,len);
	}
}

void mystart(void) 
{        
	char cmd[256];
	while(1){
		put_s("armshell$");
		get_s(cmd,256);
		parse_cmd(cmd);
	}
	
}       
  



