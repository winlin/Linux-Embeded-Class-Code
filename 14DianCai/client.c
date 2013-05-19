#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd;
	printf("Starting client...\n");
	
	if (0 > (fd=socket(AF_INET, SOCK_STREAM ,0))) {
		perror("client::socket()");
		exit(-1);
	}

	short port = 5555;
	char serverip[14] = "172.18.2.70";
	struct sockaddr_in sip;
	sip.sin_family = AF_INET;
	sip.sin_port   = htons(port);
	inet_pton(AF_INET, serverip, &sip.sin_addr);
	socklen_t len  = sizeof(sip);

	int rlen       = 0;
	char msg[1000] = {0};
	int send_len   = 0;
	
	if (connect(fd, (struct sockaddr*)&sip, len)) {
		perror("client::connect");
		exit(-1);
	}
	printf("Connect to server success:%s.\n",serverip);
	while(1) {
		printf(">: ");
		fflush(stdout);
		memset(msg, 0, sizeof(msg));
		
		if((rlen = read(0, msg, sizeof(msg))) > 0) {
			send_len = send(fd, msg, rlen, 0);
			if (0 > send_len) {
				if(errno != EINTR) {
					perror("client::send");
					exit(-1);
				} else
					continue;
			}
		}
		if (strncmp(msg, "bye", 3) == 0) {
			printf("Going to end...\n");
			exit(0);
		}
	}
	
	close(fd);
	return 0;
}
