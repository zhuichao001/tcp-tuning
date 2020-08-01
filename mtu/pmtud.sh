#! /bin/bash 
set -x

ping -c 3 -s 1472 -M do 8.8.8.8
ping -c 3 -s 1473 -M do 8.8.8.8
#From 9.134.194.61 icmp_seq=1 Frag needed and DF set (mtu = 1500)
