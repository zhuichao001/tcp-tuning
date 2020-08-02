

#tcp_max_syn_backlog <= net.core.somaxconn
echo 512 > /proc/sys/net/ipv4/tcp_max_syn_backlog

#resend SYN/ACK before reaching tcp_synack_retries times
echo 3 > /proc/sys/net/ipv4/tcp_synack_retries

#0: close, 2: open, 1: open on necessary
echo 1 > /proc/sys/net/ipv4/tcp_syncookies

netstat -l | grep 'SYNs to LISTEN sockets dropped'
