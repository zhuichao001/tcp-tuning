# TCP tuning

#### 初始设置
```
#设置可分配端口范围
echo "9000 65000"   >   /proc/sys/net/ipv4/ip_local_port_range
echo 16384  >   /proc/sys/net/core/netdev_max_backlog
```

#### 缓冲区大小
```
net.ipv4.tcp_wmem   = 4096      65536   16777216
net.ipv4.tcp_rmem   = 4096      327680  16777216
net.ipv4.tcp_mem    = 327680    327680  16777216
net.core.rmem_default   = 327680
net.core.wmem_default   = 65536
net.core.rmem_max   = 16777216
net.core.wmem_max   = 16777216
```

#### 建立连接
```
echo     3  >   /proc/sys/net/ipv4/tcp_fastopen
echo     3  >   /proc/sys/net/ipv4/tcp_syn_retries
echo     2  >   /proc/sys/net/ipv4/tcp_synack_retries

echo  2048  >   /proc/sys/net/ipv4/tcp_max_syn_backlog
echo  2048  >   /proc/sys/net/core/somaxconn
echo     1  >   /proc/sys/net/ipv4/tcp_abort_on_overflow

echo     1  >   /proc/sys/net/ipv4/tcp_syncookies
```

*TCP Backlog队列实际值 = min(tcp_max_syn_backlog, somaxconn, 应用层设置的backlog)*
#### 连接保持
```
echo   600  >   /proc/sys/net/ipv4/tcp_keepalive_time
echo     3  >   /proc/sys/net/ipv4/tcp_keepalive_probes
echo    15  >   /proc/sys/net/ipv4/tcp_keepalive_intvl
echo     5  >   /proc/sys/net/ipv4/tcp_retries2  #约30S
echo     0  >   /proc/sys/net/ipv4/tcp_slow_start_after_idle
```

#### 滑动窗口&拥塞控制
```
#设置拥塞窗口大小
ip route | while read p; do ip route change $p initcwnd 10 initrwnd 10; done
echo     1  >   /proc/sys/net/ipv4/tcp_window_scaling
```
```
echo  "cubic reno bbr"  > /proc/sys/net/ipv4/tcp_available_congestion_control
echo  bbr > /proc/sys/net/ipv4/tcp_congestion_control
```

#### 连接复用
```
echo    1   >   /proc/sys/net/ipv4/tcp_tw_reuse
echo    1   >   /proc/sys/net/ipv4/tcp_tw_recycle #NAT网络禁用
echo    0   >   /proc/sys/net/ipv4/tcp_timestamps 
```

#### 连接关闭
echo     2  >   /proc/sys/net/ipv4/tcp_fin_timeout
echo  6000  >   /proc/sys/net/ipv4/tcp_max_tw_buckets
