#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>


//Disable the Nagle (TCP Delay) algorithm
void set_sock_nodelay(int sockfd){
    int flag = 1;
    int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
    if (ret == -1) {
      printf("Couldn't setsockopt(TCP_NODELAY)\n");
      exit(-1);
    }
}


struct sockaddr_in * gen_svraddr(const char *ip, const int port) {
    struct sockaddr_in *svraddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memset (svraddr, 0, sizeof(struct sockaddr_in));
    svraddr->sin_family = AF_INET;
    svraddr->sin_addr.s_addr = inet_addr(ip);
    svraddr->sin_port = htons(port);
    return svraddr;
}


void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}


void handle(int sockfd) {
    for (int i=0; i<8; i++) {
        char line[128];
        sprintf(line, "tcp tuning nodelay, seq:%d\n\0", i);
        if (send(sockfd, line, strlen(line), 0) == -1) {
            print_errno("send");
        }
    }
    printf("After sending 8 lines.\n");
}


int main(int argc, char *argv[]) {
    int sockfd;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("socket");
    }

    set_sock_nodelay(sockfd);

    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 9997); 
    if (connect(sockfd, (struct sockaddr *)svraddr, sizeof(struct sockaddr)) == -1) {
        print_errno("connect");
    }

    sleep(30);
    close(sockfd);
    return 0;
}
