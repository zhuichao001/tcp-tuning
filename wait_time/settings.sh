

#indicates: duration of FIN_WAIT2 state
echo 'net.ipv4.tcp_fin_timeout=60' >> /etc/sysctl.conf

#notation: user can not modify MSL, unless rebuild kernel
#.../include/net/tcp.h
#define TCP_TIMEWAIT_LEN (60*HZ)
#alikernel 4.9 support sysctl_tcp_tw_timeout 


#indicates: duration of FIN_WAIT2 state
echo 'net.ipv4.tcp_tw_timeout=15' >> /etc/sysctl.conf


echo 'net.ipv4.tcp_max_tw_buckets=65536' >> /etc/sysctl.conf
