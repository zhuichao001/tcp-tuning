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

/*
 * Socket 阻塞模式下Close 时，主要的逻辑在 tcp_close() 里实现：
 *     1. 如果接收缓冲区还有数据未读，会先把接收缓冲区的数据清空，然后给对端发一个 RST
 *     2. 如果接收缓冲区是空的，那么就调用 tcp_send_fin():
 *        a. 还有些数据没发出去，内核会把发送缓冲区最后一个数据块拿出来，然后置为 FIN
 *        b. 等把发送缓冲区数据都发完，最后再执行四次挥手的第一次挥手（FIN包）
 */


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
