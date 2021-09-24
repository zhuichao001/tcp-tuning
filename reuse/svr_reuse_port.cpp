#include <netdb.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread> 
#include "common/address.h"


//linux-kernel-3.9 feature: SO_REUSEPORT
void set_reuse_port(int sfd) {
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
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

int server() { 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { 
        print_errno("socket");
    } 

    set_reuse_port(sockfd);

    struct sockaddr *svraddr = create_ipv4_addr("0.0.0.0", 8080); 
    if ((bind(sockfd, svraddr, sizeof(struct sockaddr_in))) != 0) { 
        print_errno("bind");
    } 
  
    if ((listen(sockfd, 8)) != 0) { 
        print_errno("listen");
    } 

    struct sockaddr_in cliaddr; 
    int addrlen = sizeof(cliaddr);
    while(true){
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
    return 0;
}

void wait_childs(){
    while (1) {
        pid_t ret = wait(NULL);
        if (ret == -1) {
            if (errno == EINTR) {
                continue;
            } if (errno == ECHILD) {
                fprintf(stderr, "wait error:%s\n", strerror(errno));
                continue;
            }
            break;
        }
    }

}

int main(){
    for(int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid > 0 ) {
            continue;
        } else if (pid == 0) {
            server();
            exit(0);
        } else if (pid < 0) {
            printf("fork error");
            exit(1);
        }
    }

    wait_childs();

    return 0;
}
