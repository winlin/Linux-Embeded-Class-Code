#include "protocol.h"

const char brdcast_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
const char local_mac[6]={0x0c,0x0c,0x29,0x0c,0x58,0x14};

unsigned short htons(unsigned short d)
{
	return ((d>>8)&0xff)|((d<<8)&0xff00);
}
unsigned int htonl(unsigned int d)
{
	return ((d>>24)&0xff)|((d>>8)&0xff00)|((d<<8)&0xff0000)|((d<<24)&0xff000000);
}

void mem_cpy(char* dest,const char* src,int size)
{
	int i;
	for(i=0;i<size;++i){
		dest[i]=src[i];
	}
}

void send_arp(void)
{
	char buf[64];
	struct eth_header* p=(struct eth_header*)buf;
	mem_cpy(p->dest_mac,brdcast_mac,6);
	mem_cpy(p->src_mac,local_mac,6);
	p->type=htons(0x0806);

	struct arp_header *q=(struct arp_header *)(buf+sizeof(struct eth_header));
	q->hw_type=htons(0x0001);
	q->proto_type=htons(0x0800);
	q->hw_len=6;
	q->proto_len=4;
	q->ops_code=htons(0x0001);
	mem_cpy(q->src_mac,local_mac,6);
	int local_ip=(172<<24)|(18<<16)|(2<<8)|191;
	q->src_ip=htonl(local_ip);
	mem_cpy(q->dest_mac,0,6);
	int dest_ip=(172<<24)|(18<<16)|(2<<8)|192;
	q->dest_ip=htonl(dest_ip);

	dm9000_send(buf,60);
}

