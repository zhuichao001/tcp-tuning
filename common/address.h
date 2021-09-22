#include <netdb.h> 
#include <unistd.h>
#include <stddef.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/un.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>

struct sockaddr * create_unix_domain(const char *filename) {
    struct sockaddr_un *addr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
    memset(addr, 0, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;
    //unlink(filename);
    strcpy(addr->sun_path, filename);
    return (struct sockaddr *)addr;
}

int unix_domain_size(struct sockaddr_un *addr){
    return offsetof(struct sockaddr_un, sun_path) + strlen(addr->sun_path);
}

struct sockaddr * create_ipv4_addr(const char *ip, const int port) {
    struct sockaddr_in *addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memset (addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
    return (struct sockaddr *)addr;
}
