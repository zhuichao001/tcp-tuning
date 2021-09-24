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
#include <thread> 
#include "common/address.h"


void set_reuse_addr(int sfd) {
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
}


void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}


void work(int fd) {
    for (;;) {
        char buff[128]={0,};

        int err = read(fd, buff, sizeof(buff));
        if(err<=0){
            print_errno("read");
        }
        printf("read from fd@%d : %s", err, buff);

        if (strncmp("[EXIT]", buff, 6) == 0) {
            printf("\nClient %d Exit...\n", fd); 
            break;
        }

        int n=err;
        write(fd, buff, n);
    }
}

int main() { 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { 
        print_errno("socket");
    } 

    set_reuse_addr(sockfd);

    struct sockaddr *svraddr = create_ipv4_addr("0.0.0.0", 8080); 
    if ((bind(sockfd, svraddr, sizeof(struct sockaddr_in))) != 0) { 
        print_errno("bind");
    } 
  
    if ((listen(sockfd, 8)) != 0) { 
        print_errno("listen");
    } 

    struct sockaddr_in cliaddr; 
    int addrlen = sizeof(cliaddr);
    while(1){
        int fd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t*)&addrlen); 
        if (fd < 0) { 
            print_errno("accept");
        } 
        printf("accept conn:%d\n", fd);
        std::thread h(work, fd);
        h.detach();
    }

    printf("close ...\n"); 
    sleep(1000);
}
