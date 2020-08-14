#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>


void set_tcp_defer_accept(int sockfd, int timeout) {
    setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout));
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


void server() {
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("create");
    }

    set_tcp_defer_accept(listenfd, 5);
 
    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 9898); 
    if (bind(listenfd, (struct sockaddr *)svraddr, sizeof (struct sockaddr_in)) == -1) {
        print_errno("bind");
    }
 
    int backlog = 10;
    if (listen(listenfd, backlog) == -1) {
        print_errno("listen");
    }
 

    int fd;
    while (1) {
        printf("Before accpet.\n");
        if ((fd = accept (listenfd, (struct sockaddr *) NULL, NULL)) == -1) {
            printf("accept error: %s(errno: %d)\n", strerror(errno), errno);
        }
        
        char recv_buf[1024];
        recv(fd, recv_buf, sizeof(recv_buf), 0);
        printf("|=====> recv: %s\n", recv_buf);
        close(fd);
    }
}


int main(){
    server();
}
