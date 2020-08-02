#! /bin/bash

#indicates: retry 8 times, take effect for all FIN_WAIT1 or LAST_ACK state
echo 'net.ipv4.tcp_orphan_retries=0' >> /etc/sysctl.conf

#cause of FIN_WAIT1
#   1.malicious attacks
#   2.buffer data
#   3.window zero

#cause of CLOSE_WAIT:
#    1: lack close()
#    2: load too high

#solve, if reach maxnum, send RST
echo 'net.ipv4.tcp_max_orphans=1024' >> /etc/sysctl.conf

