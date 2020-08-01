#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>


void tfo_client(){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf ("create socket error: %s(errno: %d)\n", strerror (errno), errno);
        exit (0);
    }
 
    struct sockaddr_in servaddr;
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8764);
 
    char buf[] = "hello, this is tcp fast open test data...";
    int ret = sendto(sockfd, buf, strlen(buf), MSG_FASTOPEN, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        printf ("send msg error: %s(errno: %d)\n", strerror (errno), errno);
    }
    close(sockfd);
}


int main(){
    tfo_client();
}
