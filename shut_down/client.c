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


int process(int sockfd) {
    char line[] = "tcp tuning shut down.";
    if (send(sockfd, line, sizeof(line), 0) == -1) {
        print_errno("send");
    }
    printf("Close write pipe after sending:%s \n", line);
    shutdown(sockfd, SHUT_WR);

    sleep(3); //do other thing

    int bytes;  
    char buf[256] = {0,};
    if ((bytes=recv(sockfd, buf, 256, 0)) == -1) {
        print_errno("recv");
    }
    printf("Close read pipe after receive: %s\n", buf);
    shutdown(sockfd, SHUT_RD);
}


int main(int argc, char *argv[]) {
    int sockfd;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("socket");
    }

    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 9996); 

    if (connect(sockfd, (struct sockaddr *)svraddr, sizeof(struct sockaddr)) == -1) {
        print_errno("connect");
    }

    process(sockfd);
    return 0;
}
