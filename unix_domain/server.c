#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "common/address.h"

#define MYPORT 3576
#define BACKLOG 128

const char * UNIX_DOM_SOCK_PATH = "./domsock.unix";

int main() {
    const  int N = 1024;
    int    fds[N];
    int    num = 0;

    int    svrsockfd = -1;
    if ((svrsockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    fcntl(svrsockfd, F_SETFL, O_NONBLOCK);
    fds[num++] = svrsockfd;

    unlink(UNIX_DOM_SOCK_PATH);
    struct sockaddr *addr = create_unix_domain(UNIX_DOM_SOCK_PATH);
    
    if (bind(svrsockfd, addr, unix_domain_size(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(svrsockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    while (1) {
        int i;
        for (i=0; i<=num; ++i) {
            int sockfd = fds[i];
            if (fds[i] == svrsockfd) {
                struct sockaddr_un peer_addr;
                int sin_size = unix_domain_size(addr);
                int new_fd = -1;
                if ((new_fd = accept(sockfd, (struct sockaddr *)&peer_addr, &sin_size)) == -1) {
                    perror("accept");
                }
                fcntl(new_fd, F_SETFL, O_NONBLOCK);
                fds[num++] = new_fd;
            } else {
                char buff_read[256];
                int n = recv(fds[i], buff_read, sizeof(buff_read),0);
                if (n < 1) {  //TODO: deal closed fd
                    perror("recv - non blocking \n");
                    printf("data read size is: n=%d \n", n);
                } else {
                    buff_read[n] = '\0';
                    printf("The string is: %s \n",buff_read);
                    if (send(fds[i], "Hello, world!\n", 14, 0) == -1) {
                        perror("send");
                    }
                }
            }
            usleep(500*1000);
        }
    }
    return 0;
}
