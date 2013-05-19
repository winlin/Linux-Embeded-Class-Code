struct eth_header{
	char dest_mac[6];
	char src_mac[6];
	unsigned short type;	
}__attribute__((packed));

struct arp_header{
	unsigned short hw_type;
	unsigned short proto_type;
	unsigned char hw_len;
	unsigned char proto_len;
	unsigned short ops_code;
	char src_mac[6];
	unsigned int src_ip;
	char dest_mac[6];
	unsigned int dest_ip;
}__attribute__((packed));
