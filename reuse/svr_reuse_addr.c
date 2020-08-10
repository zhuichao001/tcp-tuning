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


void set_reuse_addr(int sfd) {
    int reuse = 1;
    socklen_t socklen = sizeof(reuse);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, socklen);
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

    set_reuse_addr(sockfd);

    struct sockaddr_in *svraddr = gen_svraddr("0.0.0.0", 8080); 
    if ((bind(sockfd, (struct sockaddr *)svraddr, sizeof(*svraddr))) != 0) { 
        print_errno("bind");
    } 
  
    if ((listen(sockfd, 8)) != 0) { 
        print_errno("listen");
    } 

    struct sockaddr_in cliaddr; 
    int addrlen = sizeof(cliaddr);
    int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t*)&addrlen); 
    if (connfd < 0) { 
        print_errno("accept");
    } 

    serve(connfd); 

    printf("close ...\n"); 
    sleep(1000);
}
