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


void tfo_server(){
    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit (0);
    }
 
    struct sockaddr_in servaddr;
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons (8764);
 
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
 
    int backlog = 10;
    if (listen(listenfd, backlog) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
 
    int qlen = 5;
    if (setsockopt(listenfd, IPPROTO_TCP, TCP_FASTOPEN, &qlen, sizeof(qlen)) < 0) {
        printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
    }

    while (1) {
        printf("Before accpet!\n");
        int fd;
        if ((fd = accept (listenfd, (struct sockaddr *) NULL, NULL)) == -1) {
            printf("accept error: %s(errno: %d)\n", strerror(errno), errno);
        }
        
        char recv_buf[1024];
        recv(fd, recv_buf, sizeof(recv_buf), 0);
        printf("=====> recv: %s\n", recv_buf);
        close(fd);
    }

}


int main(){
    tfo_server();
}
