#include <netdb.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>


void set_socket_nolinger(int sfd) {
    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger = 0;
    setsockopt(sfd, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger));
}


struct sockaddr_in * gen_svraddr(const char *ip, const int port) {
    struct sockaddr_in *svraddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memset(svraddr, 0, sizeof(struct sockaddr_in));
    svraddr->sin_family = AF_INET;
    svraddr->sin_addr.s_addr = inet_addr(ip);
    svraddr->sin_port = htons(port);
    return svraddr;
}


void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}


void serve(int sockfd) { 
	for (;;) { 
        char buff[128]={0,}; 

		int err = read(sockfd, buff, sizeof(buff)); 
        if(err<=0){
            print_errno("read");
        }

        int n=0; 
		printf("From client: %s\n", buff); 
		while ((buff[n++] = getchar()) != '\n') ; 
        buff[n]=0;

		write(sockfd, buff, sizeof(buff)); 

		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	}
} 

int main() { 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0) { 
        print_errno("socket");
	} 
  
    set_socket_nolinger(sockfd);

    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 8080); 
	if ((bind(sockfd, (struct sockaddr *)svraddr, sizeof(*svraddr))) != 0) { 
        print_errno("bind");
	} 

	if ((listen(sockfd, 5)) != 0) { 
        print_errno("listen");
	} 

	struct sockaddr_in cliaddr; 
	int addrlen = sizeof(cliaddr);
	int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t*)&addrlen); 
	if (connfd < 0) { 
        print_errno("accept");
	} 

    set_socket_nolinger(connfd);
	serve(connfd); 

	printf("close client...\n"); 
    sleep(1000);
} 
