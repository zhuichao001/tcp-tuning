#include <string>
#include <stdio.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

int get_localip(const std::string eth_name, std::string &local_ip_addr) {
    int fd, intrface;
    struct ifreq ifr[32];
    struct ifconf ifc;

    if ((fd=socket(AF_INET, SOCK_DGRAM, 0)) > 0) {
        ifc.ifc_len = sizeof ifr;
        ifc.ifc_buf = (caddr_t)ifr;
        //get all interface infomation
        if (!ioctl(fd, SIOCGIFCONF, (char*)&ifc)) { 
            intrface = ifc.ifc_len / sizeof(struct ifreq);
            while (intrface-- > 0) {
                //Get IP Address
                if (!(ioctl(fd, SIOCGIFADDR, (char*)&ifr[intrface]))) {
                    if(strcmp(eth_name.c_str(), ifr[intrface].ifr_name) == 0) {
                        local_ip_addr = inet_ntoa(((struct sockaddr_in*)(&ifr[intrface].ifr_addr))->sin_addr);
                        break;
                    }
                }
            }
        }
    }

    if (fd > 0) {
        close(fd);
    }
    return 0;
}

int main(int argc, const char **argv) {
    std::string local_ip;

    int ret = get_localip("eth0", local_ip);
    if (ret == 0) {
        printf("local ip:%s\n", local_ip.c_str());
    } else {
        printf("get local ip failure\n");
    }
    return 0;
}
