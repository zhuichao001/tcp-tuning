#! /bin/bash 
set -x

ping -c 3 -s 1472 -M do 8.8.8.8
#76 bytes from 8.8.8.8: icmp_seq=1 ttl=109 (truncated)

ping -c 3 -s 1473 -M do 8.8.8.8
#From 9.134.194.61 icmp_seq=1 Frag needed and DF set (mtu = 1500)

#In conlusion, MTU:1500 = 20 | 8 | 1472
