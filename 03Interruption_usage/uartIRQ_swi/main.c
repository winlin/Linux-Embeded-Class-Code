
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
		
		char recv_msg[1536];
		unsigned int len;
		len=dm9000_recv(recv_msg);
		if(len==0)
			put_s("Recieve no data!\n");
		else
			print_recv_arp(recv_msg,len);
	}
	else if(!str_equl(cmd,"swi_test")){
			swi_trigger();
	}
	else if(cmd[0])
		put_s("Command not found.\n");
}

void mystart(void) 
{        
	char cmd[256];
	enable_irq();
	while(1){
		put_s("armshell$");
		get_s(cmd,256);
		parse_cmd(cmd);
	}
	
} 
