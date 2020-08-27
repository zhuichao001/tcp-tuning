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

#define MYPORT 3576
#define BACKLOG 128


int main() {
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int    sin_size;
    const  int N = 1024;
    int    fds[N];
    int    num = 0;

    int    svrsockfd = -1;
    if ((svrsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    fcntl(svrsockfd, F_SETFL, O_NONBLOCK);
    fds[num++] = svrsockfd;

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);
    
    if (bind(svrsockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(svrsockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    int new_fd = -1;
    if ((new_fd = accept(svrsockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
        perror("accept");
    }
    fcntl(new_fd, F_SETFL, O_NONBLOCK);

    while (1) {
        int i;
        for (i=0; i<=num; ++i) {
            int sockfd = fds[i];
            if (fds[i] == svrsockfd) {
                sin_size = sizeof(struct sockaddr_in);
                if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
                    perror("accept");
                }
                fcntl(new_fd, F_SETFL, O_NONBLOCK);
                //printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr)); 
                fds[num++] = new_fd;
            } else {
                char buff_read[256];
                int n = recv(new_fd,buff_read,sizeof(buff_read),0);
                if (n < 1) { 
                    perror("recv - non blocking \n");
                    printf("data read size is: n=%d \n", n);
                } else {
                    buff_read[n] = '\0';
                    printf("The string is: %s \n",buff_read);
                    if (send(new_fd, "Hello, world!\n", 14, 0) == -1) {
                        perror("send");
                    }
                }
            }
            usleep(500*1000);
        }
    }
    return 0;
}
