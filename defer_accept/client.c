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


void set_tcp_defer_accept(int sockfd, int timeout) {
    setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout));
}

void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}

int main(int argc, char *argv[]) {
    int sockfd;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("socket");
    }

    set_tcp_defer_accept(sockfd, 5);

    struct sockaddr *svraddr = create_ipv4_addr("127.0.0.1", 9898); 

    if (connect(sockfd, svraddr, sizeof(struct sockaddr_in)) == -1) {
        print_errno("connect");
    }

    sleep(10);

    while (1) {
        char line[] = "tcp tuning test defer-accept.";
        if (send(sockfd, line, sizeof(line), 0) == -1) {
            print_errno("send");
        }
        printf("After sending:%s \n", line);

        int bytes;  
        char buf[256] = {0,};
        if ((bytes=recv(sockfd, buf, 256, 0)) == -1) {
            print_errno("recv");
        }

        printf("[pid=%d], Receive: %s\n", getpid(), buf);
        const int IDLE_SECONDS = 120;
        sleep(IDLE_SECONDS);
    }

    close(sockfd);
    return 0;
}
