#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>


struct sockaddr_in * gen_svraddr(const char *ip, const int port){
    struct sockaddr_in *svraddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memset(svraddr, 0, sizeof(struct sockaddr_in));
    svraddr->sin_family = AF_INET;
    svraddr->sin_addr.s_addr = inet_addr(ip);
    svraddr->sin_port = htons(port);
    return svraddr;
}


void print_errno(const char * prefix){
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}


void tfo_client(){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        print_errno("create");
    }
 
    struct sockaddr_in * svraddr = gen_svraddr("127.0.0.1", 8766); 
    char buf[] = "hello, coming TFO data.";
    if (sendto(sockfd, buf, strlen(buf), MSG_FASTOPEN, (struct sockaddr *)svraddr, sizeof(*svraddr)) < 0) {
        printf ("send msg error: %s(errno: %d)\n", strerror (errno), errno);
    }
    close(sockfd);
}


int main(){
    tfo_client();
}
