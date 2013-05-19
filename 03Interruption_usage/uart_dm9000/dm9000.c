#define DM9000_INDEX 0x18000000
#define DM9000_DATA  0x18000004

/* delay function */
void delay_fun(int n)
{
	volatile int i;
	for(i=0;i<0xffff*n;++i){
		i=i-23;i=i+23;i=i;
	}
}
/* Get the unsigned char value of index-th register */
unsigned char dm9000_reg_read(unsigned char index)
{
	volatile unsigned char* p=(unsigned char*)DM9000_INDEX;
	*p=index;
	p=(unsigned char*)DM9000_DATA;
	return *p;
}
/* Get the unsigned short value of index-th register */
unsigned short dm9000_reg_read_short(unsigned char index)
{
	volatile unsigned char* p=(unsigned char*)DM9000_INDEX;
	*p=index;
	p=(unsigned char*)DM9000_DATA;
	return *((unsigned short*)p);
}
/* Write char data into index-th register */
void dm9000_reg_write(unsigned char index,char data)
{
	volatile unsigned char* p=(unsigned char*)DM9000_INDEX;
	*p=index;
	p=(unsigned char*)DM9000_DATA;
	*p=data;
}
/* Write short data into index-th register */
void dm9000_reg_write_short(unsigned char index,unsigned short data)
{
	volatile unsigned char* p=(unsigned char*)DM9000_INDEX;
	*p=index;
	p=(unsigned char*)DM9000_DATA;
	*((unsigned short*)p)=data;
}

/* read MAC from EEPROM */
void dm9000_read_mac(char* src)
{
	int i;
	for(i=0;i<3;++i){
		dm9000_reg_write(0x0c,i);
		dm9000_reg_write(0x0b,0x4);
		/* at lease delay 150us */
		delay_fun(6);
		dm9000_reg_write(0x0b,0x0);
		src[i*2]=dm9000_reg_read(0x0d);
		src[i*2+1]=dm9000_reg_read(0x0e);
	}
	return ;
}
/* General Purpose dm9000 reset routine */
void dm9000_reset(void)
{
	dm9000_reg_write(0x00,0x00);
	delay_fun(6);
	dm9000_reg_write(0x00,0x00);
	delay_fun(6);
}

/* initlize the DM9000 */
void dm9000_init()
{
	/* Reset the network */
	dm9000_reset();

	/* active the internel PHY */
	dm9000_reg_write(0x00,0x8);  /* Enable full-duplex mode of the internel PHY */
	/* GPIO0 on pre-activate PHY  */
	dm9000_reg_write(0x1f,0x00);  /* REG_1F bit0 activate phyxcer*/
	delay_fun(6);

	/* Porgram operating register */
       /* set NCR register normal mode */
	dm9000_reg_write(0x00,0x0);
	dm9000_reg_write(0x02,0);     /* TX Polling clear */
	/* clear TX NSR 0x01h */
	dm9000_reg_write(0x01,0x2c);
	/* Clear iterrupt status */
	dm9000_reg_write(0xfe,0x0f);
	
	/* read EEPROM data */
	char mac[6];
	dm9000_read_mac(mac);
	/* stroe MAC into 0x10-15H register */
	int i;
	for(i=0x10;i<=0x15;++i){
		dm9000_reg_write(i,mac[i-0x10]);
	}

	/* multicast address use default */
	/* TODO ... */
	for(i=0x16;i<=0x1d;++i){
		dm9000_reg_write(i,0x00);
	}

	/* Set IMR register enabke SRAM read/write pointer */
	dm9000_reg_write(0xff,0x80);
	
	/* Depend on OS and DDK of the system to */
	/* handle NIC interrupts. */
	/* TODO ... */

	/* ebable interrupt for reciev/transmitted a package */
	/* First you must registe the interrupt handler */
	//dm9000_reg_write(0xff,0x3);

	/* enable RX RAM */
	dm9000_reg_write(0x5,0x1);
	/* Enable TX/RX interrupt mask */
	dm9000_reg_write(0xff,0x83);

	/* justic the network is workabke */
	unsigned char linked=dm9000_reg_read(0x01);
	if(linked&(1<<7))
		put_s("GOOD : NETWORK LINKED\n");
	else{
		put_s("WRONG : NETWORK NOT LINKED\n");
		put_s("Network Status Register (01H): ");
		print_char_hex(linked);
		put_c('\n');
	}
	return ;
}

/* Get the vendor ID and Product ID */
int get_dm9000_id()
{
	int value=0;
	char* p=(char*)&value;
	char cc=dm9000_reg_read(0x28);
	*p=cc;
	cc=dm9000_reg_read(0x29);
	*(++p)=cc;
	cc=dm9000_reg_read(0x2a);
	*(++p)=cc;
	cc=dm9000_reg_read(0x2b);
	*(++p)=cc;
	
	return value;
}

/* Send a Packet */
int dm9000_send(char* buf,unsigned short len)
{
	/* 1. justic the iomode */
	char iomode=dm9000_reg_read(0xfe)>>7;
	/* 2. Write data into 0xF8 register */
	volatile unsigned char *addr=(unsigned char *)DM9000_INDEX;
	*addr=0xf8;
	if(iomode==1){
		//8bits
		int i;
		addr=(unsigned char*)DM9000_DATA;
		for(i=0;i<len;++i){
			*addr=buf[i];
		}
	}
	else if(iomode==0){
		//16bits
		int tmp=(len+1)>>1;
		int i;
		addr=(unsigned char *)DM9000_DATA;
		for(i=0;i<tmp;++i){
			*(unsigned short *)addr=((unsigned short *)buf)[i];
		}
	}
	/* 3.Write the data's length */
	dm9000_reg_write(0xfd,0xff&(len>>8));
	dm9000_reg_write(0xfc,0xff&len);
	/* 4.Set the send flag */
	dm9000_reg_write(0x2,1);
	/* 5.Check if send successfully */
	char ret=dm9000_reg_read(0xfe);
	if(ret&2){
		dm9000_reg_write(0xfe,2);
		return 0;
	}
	else{
		return 1;
	}
}
/* return the length of recieve data */
unsigned int dm9000_recv(char* dest_buf)
{
	unsigned int data_len=0;
        /* 1.check id recieve packets */
	unsigned char recv_flag=dm9000_reg_read(0xfe);
	if(recv_flag&1){
		while(!data_len){
			/* 2.justic if can read packet:0xF0h*/
			unsigned char is_read=dm9000_reg_read(0xf0);
			if(is_read==1){
				put_s("Recieving Packet... \n");
				unsigned char iomode=dm9000_reg_read(0xfe)>>7;
				volatile unsigned char* addr=(unsigned char*)DM9000_INDEX;
				*addr=0xf2;
				addr=(unsigned char*)DM9000_DATA;
				if(iomode==1){  //8bits
					is_read=*addr;
					unsigned char status=*addr;
					unsigned char low_len=*addr;
					data_len=*addr;
					data_len=(data_len<<8)&0xff00+low_len;
					
					unsigned int i;
					/* 3.read data from 0xf2 */
					for(i=0;i<data_len;++i){
						dest_buf[i]=*addr;
					}
				}
				else if(iomode==0){ //16bits
					unsigned short frist_two_byte=*((unsigned short*)addr);
					data_len=*((unsigned short*)addr);    /* no convertion!! */

					unsigned int times=(data_len+1)>>1;
					unsigned int i;
					for(i=0;i<times;++i){
						((unsigned short*)dest_buf)[i]=*((unsigned short*)addr);
					}
				}
			}
			else if(is_read!=0){
				put_s("Error occur Reset the DM9000!!!\n");
				dm9000_reg_write(0xff,0x80); /* stop INT request */
				dm9000_reg_write(0xfe,0x0f); /* recieve overflow counter overflow*/
				dm9000_reg_write(0x05,0x00); /* stop RX function */
				dm9000_reg_write(0x00,0x01); /* software reset and auto clear after 10us*/
			}
		}
		/* clear bit 0 to latched in ISR,*/
		/*after this can't to read next packet,so dont commit*/
		//dm9000_reg_write(0xfe,1);
	}
	return data_len;
}

void print_recv_arp(char* msg,unsigned int len)
{
	put_s("Recieve Data with Length ");
	print_int_hex(len);
	put_c('\n');
	
	int i;
	char c_str[3];
	for(i=0;i<len;++i){
		ctoa(msg[i],c_str);
		put_s(c_str);
		put_c(' ');
		if((i+1)%8==0)
			put_c('\n');
	}
	put_c('\n');
}
