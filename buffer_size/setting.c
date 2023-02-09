#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include<netinet/in.h>
#include<arpa/inet.h>

//https://sandilands.info/sgordon/impact-of-bandwidth-delay-product-on-tcp-throughput

int get_BDP(){
    int band_width = 1024*1024;
    float rtt = 0.01;
    return band_width * rtt;
}

void setting_check_recvbuf(){
    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    int expect_buf_size = get_BDP();

    //SO_SNDBUF for sending buffer
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&expect_buf_size, sizeof(expect_buf_size));

    int real_buf_size;
    socklen_t optlen = sizeof(real_buf_size);
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &real_buf_size, &optlen);
    printf("expect buffer size=%d, assigned buffer size=%d\n", expect_buf_size, real_buf_size);
}

int main() {
    setting_check_recvbuf(); 
}
