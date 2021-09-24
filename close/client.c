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
#include "common/address.h"


void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}

//if recv buf is not empty, send RST
int process(int sockfd) {
    char buf[256] = {0,};
    int n;  
    if ((n=recv(sockfd, buf, 256, 0)) == -1) {
        print_errno("recv");
        return -1;
    }
    printf("receive: %s\n", buf);

    sleep(10);
    close(sockfd);
    return 0;
}

int main(int argc, char *argv[]) {
    int sockfd;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("socket");
    }

    struct sockaddr *svraddr = create_ipv4_addr("127.0.0.1", 9996); 

    if (connect(sockfd, svraddr, sizeof(struct sockaddr_in)) == -1) {
        print_errno("connect");
    }

    process(sockfd);
    sleep(300);
    return 0;
}
