#! /bin/bash

set -x

#Recv-Q: waitting accept, Send-Q: queue size
ss -nlt

#temporaryly
sysctl -w net.core.somaxconn = 1024

#permanently adjust 
echo 'net.core.somaxconn= 1024' >> /etc/sysctl.conf

#notation: accept(svrfd, backlog)
#complete queue size: MIN(somaxconn, backlog)

#abort on over flow(RST)
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow

#to see how much is droped
netstat -s | grep TCPBacklogDrop

#to see how much overflowed
netstat -s | grep overflowed
