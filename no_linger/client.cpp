#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>


void set_socket_nolinger(int sfd) {
    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger = 2;
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


void exchange(int sockfd) { 
    for (;;) { 
        char buff[128]; 
        int n=0; 

        printf("Enter: "); 
        while ((buff[n++] = getchar()) != '\n'); 
        buff[n]=0;

        write(sockfd, buff, sizeof(buff)); 

        int err = read(sockfd, buff, sizeof(buff)); 
        if(err<=0){
            print_errno("read");
        }

        printf("From Server : %s\n", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
    close(sockfd); 
} 
  
int main() { 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        print_errno("socket");
    } 

    set_socket_nolinger(sockfd);

    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 8080); 
    if (connect(sockfd, (struct sockaddr *)svraddr, sizeof(*svraddr)) != 0) { 
        print_errno("connect");
    } 

    printf("connected to the server..\n"); 
    exchange(sockfd); 
} 
