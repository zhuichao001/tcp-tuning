#include <netdb.h> 
#include <unistd.h>
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>


unsigned short checksum (unsigned short *addr, int len) {
    int sum=0;
    unsigned short res=0;
    while( len > 1)  {
        sum += *addr++;
        len -=2;
        printf("sum is %x.\n",sum);
    }
    if( len == 1) {
        *((unsigned char *)(&res))=*((unsigned char *)addr);
        sum += res;
    }
    sum = (sum >>16) + (sum & 0xffff);
    sum += (sum >>16) ;
    res = ~sum;
    return res;
}

void display_reply (struct sockaddr_in *from, const char *recvbuff, const int n) {
        struct ip *ip=(struct ip *)recvbuff;
        struct icmp *icmp = (struct icmp*)(ip+1);
        printf("n is %d,ip header length is %d\n ",n,ip->ip_hl);
        if((n-ip->ip_hl*4)<8) {
            printf("Not ICMP Reply!\n");
            return;
        }
        printf("ttl is %d\n",ip->ip_ttl);    
        printf("protocol is %d\n",ip->ip_p);
        if((icmp->icmp_type==ICMP_ECHOREPLY)&&(icmp->icmp_id==2)){
            printf("reply from %s: seq=%u ttl=%d\n",inet_ntoa(from->sin_addr),icmp->icmp_seq,ip->ip_ttl);
        }
        printf("src is %s\n",inet_ntoa(ip->ip_src));
        printf("dst is %s\n",inet_ntoa(ip->ip_dst));
}

void ping (const char *ip, int times) {
    struct sockaddr_in target, from;
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    const int PING_CMD_LEN = 8;
    char sendbuff[PING_CMD_LEN];
    char recvbuff[1024]={0,};
    struct icmp * icmp = (struct icmp*)sendbuff;
    
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_id = 2;
    icmp->icmp_seq = 3;
    
    printf("sendbuff:[%x]\n", *sendbuff);
    if (inet_aton(ip, &target.sin_addr)==0) {
        printf("bad address: %s\n", ip);
        exit(1);
    }
    
    while (times--) {
        icmp->icmp_seq += 1;
        icmp->icmp_cksum = checksum((unsigned short *)icmp, PING_CMD_LEN);
        sendto(sockfd, sendbuff, PING_CMD_LEN, 0, (struct sockaddr *)&target, sizeof(target));

        socklen_t len_sockaddr = sizeof(struct sockaddr_in);
        int n = recvfrom(sockfd, recvbuff,sizeof(recvbuff), 0, (struct sockaddr *)&from, &len_sockaddr);
        if (n<0) {
            perror("receive error!\n");
            exit(1);
        }    
        display_reply(&from, recvbuff, n);
    }

    close(sockfd);
}

int main (int argc, char ** argv) {
    if(argc!=2) {
        printf("Usage:%s targetip",argv[0]);
        exit(1);
    }

    ping(argv[1], 1);
    return 0;
}
