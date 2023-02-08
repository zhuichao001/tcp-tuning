# TCP tuning
配置项位置说明：
net.core.{option} = /proc/sys/net/core/{option}
net.ipv4.{option} = /proc/sys/net/ipv4/{option}

#### 初始设置
```
#设置可分配端口范围
net.ipv4.ip_local_port_range = 9000 65000
#TODO
net.core.netdev_max_backlog = 16384
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
net.ipv4.tcp_fastopen = 3
net.ipv4.tcp_syn_retries = 3
net.ipv4.tcp_synack_retries = 2

net.core.somaxconn = 16384
net.ipv4.tcp_max_syn_backlog = 16384
net.ipv4.tcp_abort_on_overflow = 1
net.ipv4.tcp_syncookies = 1
```
*TCP Backlog队列实际值 = min(tcp_max_syn_backlog, somaxconn, 应用层设置的backlog)*

#### 连接保持
```
net.ipv4.tcp_keepalive_time = 600
net.ipv4.tcp_keepalive_probes = 3
net.ipv4.tcp_keepalive_intvl = 15
net.ipv4.tcp_retries2 = 5
net.ipv4.tcp_slow_start_after_idle = 0
```

#### 滑动窗口&拥塞控制
```
#设置拥塞窗口大小
ip route | while read p; do ip route change $p initcwnd 10 initrwnd 10; done
net.ipv4.tcp_window_scaling = 1
```
```
net.ipv4.tcp_available_congestion_control = cubic reno bbr
net.ipv4.tcp_congestion_control = bbr
```

#### 连接回收
```
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_tw_recycle = 0 #NAT网络禁用
net.ipv4.tcp_timestamps = 0

net.ipv4.tcp_fin_timeout = 2
net.ipv4.tcp_max_tw_buckets = 6000
```
