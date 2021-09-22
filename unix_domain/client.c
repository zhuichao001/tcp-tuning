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

const char * UNIX_DOM_SOCK_PATH = "./domsock.unix";


void print_errno(const char * prefix) {
     printf("%s socket error: %s(errno: %d)\n", prefix, strerror(errno), errno);
     exit (0);
}


int main(int argc, char *argv[]) {
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) == -1) { //SOCK_SEQPACKET instead of SOCK_STREAM
        print_errno("socket");
    }

    struct sockaddr * addr = create_unix_domain(UNIX_DOM_SOCK_PATH); 

    if (connect(fd, (struct sockaddr *)addr, unix_domain_size(addr)) == -1) {
        print_errno("connect");
    }

    while (1) {
        char line[] = "tcp socket look whether seqpacket.";
        if (send(fd, line, sizeof(line), 0) == -1) {
            print_errno("send");
        }
        printf("After sending:%s \n", line);

        int bytes;  
        char buf[256] = {0,};
        if ((bytes=recv(fd, buf, 256, 0)) == -1) {
            print_errno("recv");
        }

        printf("[pid=%d], Receive: %s\n", getpid(), buf);
    }

    close(fd);
    return 0;
}
