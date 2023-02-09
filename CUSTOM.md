## 预防SYNC FLOOD
```
sysctl -w net.ipv4.tcp_max_syn_backlog="16384" 
sysctl -w net.ipv4.tcp_synack_retries="1" 
sysctl -w net.ipv4.tcp_max_orphans="400000"
```
