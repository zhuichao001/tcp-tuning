
#for linux kernel > 3.7.1

#1 enable client, 2 enable server 
echo 3 > /proc/sys/net/ipv4/tcp_fastopen
systemctl restart network

YOUR_SITE='http://10.140.10.16/'
curl -s --tcp-fastopen $YOUR_SITE

ip tcp_metrics show | grep "fo_cookie"
