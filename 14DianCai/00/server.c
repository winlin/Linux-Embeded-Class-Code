#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>

void child_handle(int signum)
{
	signal(signum, child_handle);
	while(waitpid(-1, NULL, 0) > 0);
	return ;
}

int main(int argc, char **argv)
{
	int       sfd;
	short     port = 5555;
	struct sockaddr_in sip;
	sip.sin_family = PF_INET;
	sip.sin_port   = htons(port);
	sip.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("Server is starting ......\n");
	signal(SIGCHLD, child_handle);
	
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		perror("server::socket()");
		exit(-1);
	}

	/* Set the ip is reusable */
	int  i = 1;
	if (0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i))) {
		perror("server::setsockopt()");
		exit(-1);
	}
	
	if (bind(sfd, (struct sockaddr *)&sip, sizeof(sip)) < 0) {
		perror("server::bind()");
		exit(-1);
	}

	printf("Server started and wait for conneted...\n");
	if(0 > listen(sfd, 10)) {
		perror("server::listen");
		exit(-1);
	}

	struct sockaddr_in cip;
	socklen_t len = sizeof(cip);

	int client_fd;
	int rlen;
	char ip[100];
	char msg[1000];
	while(1) {
		if((client_fd = accept(sfd, (struct sockaddr *)&cip, &len)) < 0) {
			if ( errno != EINTR) {
				perror("server::accept()");
				exit(-1);
			} else
				continue;
		}
		if (fork() != 0) {
			inet_ntop(PF_INET, &cip.sin_addr, ip, sizeof(ip));
			printf("%s is connected!\n",ip);
			sprintf(msg, "your ip is %s\n",ip);
			if (0 > send(client_fd, msg, sizeof(msg),0)) {
				perror("server::send()");
				exit(-1);
			}
			memset(msg, 0, sizeof(msg));
			memset(ip, 0, sizeof(ip));
			
			continue;
		}

		//close(sfd);
		int send_len;
		while(1) {
			if( (rlen = recv(client_fd, msg, sizeof(msg), MSG_DONTWAIT)) >0) {
				printf("\nRecieve Msg: %s\n",msg);
				memset(msg, 0, sizeof(msg));
			} else {
				perror("client::send");
				exit(-1);
			}
			
			printf(">: ");
			if((rlen = read(0, msg, sizeof(msg))) > 0) {
				send_len = send(client_fd, msg, rlen, 0);
				if (0 > send_len) {
					if(errno != EINTR) {
						perror("client::send");
						exit(-1);
					} else
						continue;
				}
			}
			if (strncmp(msg, "bye", 3) == 0)
				exit(0);
		}
		
	}
	return 0;
}
