#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include<netinet/in.h>
#include <netinet/tcp.h>
#include<arpa/inet.h>


void set_socket_nolinger(int sfd) {
    struct linger linger;
    linger.l_onoff = 0;
    linger.l_linger = 0; //imediately send RST
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


int main(int argc, char *argv[]) {
    int sockfd;  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        print_errno("socket");
    }

    set_socket_nolinger(sockfd);

    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 9990); 

    if (connect(sockfd, (struct sockaddr *)svraddr, sizeof(struct sockaddr)) == -1) {
        print_errno("connect");
    }

    char line[] = "tcp tuning RST.";
    if (send(sockfd, line, sizeof(line), 0) == -1) {
        print_errno("send");
    }
    printf("After sending:%s \n", line);

    sleep(1);
    close(sockfd);
    return 0;
}
