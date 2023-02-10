# Linux 内核 TCP 参数调优
配置项位置说明： 
```
net.core.{option} = /proc/sys/net/core/{option}
net.ipv4.{option} = /proc/sys/net/ipv4/{option}
```

#### 如何修改参数：  
```
#临时设置-系统参数文件：
echo "{SOME_VALUE}" > /proc/sys/net/ipv4/tcp_xxx

#临时设置-执行命令:
systemctl -w net.core.{option} = {value}

#永久生效-需要修改: /etc/sysctl.conf
sysctl -p

```
> 有些参数的生效需要刷新路由信息: sysctl -w net.ipv4.route.flush=1

#### 性能验证：   
```
dd if=/dev/urandom of=sample.txt bs=1M count=1024 iflag=fullblock
scp sample.txt your_username@remotehost.com:/some/remote/directory
```

#### 查看统计:  
```
ls /sys/class/net/eth0/statistics/
```

#### 初始设置
```
#设置可分配端口范围
net.ipv4.ip_local_port_range = 9000 65000
#内核从NIC收到包后,交由协议栈(如IP、TCP)处理之前的缓冲队列长度
net.core.netdev_max_backlog = 16384
```

#### 窗口 | 缓冲区大小
```
net.ipv4.tcp_mem        = 327680    327680      16777216
net.ipv4.tcp_wmem       = 4096      65536       16777216
net.ipv4.tcp_rmem       = 4096      327680      16777216
net.core.rmem_default   = 327680
net.core.wmem_default   = 65536
net.core.rmem_max       = 16777216
net.core.wmem_max       = 16777216

#设置网卡队列大小
ifconfig eth0 txqueuelen 1000
```

#### 建立连接
```
net.ipv4.tcp_fastopen           = 3
net.ipv4.tcp_syn_retries        = 2
net.ipv4.tcp_synack_retries     = 2

net.core.somaxconn              = 16384 #全连接队列
net.ipv4.tcp_max_syn_backlog    = 16384 #半连接队列
net.ipv4.tcp_abort_on_overflow  = 1
net.ipv4.tcp_syncookies         = 1
```
*TCP Backlog队列实际值 = min(tcp_max_syn_backlog, somaxconn, 应用层设置的backlog)*

#### 连接保持
```
net.ipv4.tcp_keepalive_time     = 600
net.ipv4.tcp_keepalive_probes   = 3
net.ipv4.tcp_keepalive_intvl    = 15
net.ipv4.tcp_retries1           = 3
net.ipv4.tcp_retries2           = 5
net.ipv4.tcp_no_delay_ack       = 1
net.ipv4.tcp_low_latency        = 1

```

#### 拥塞控制
```
#设置拥塞窗口大小
ip route | while read p; do ip route change $p initcwnd 10 initrwnd 10; done
net.ipv4.tcp_window_scaling = 1

#空间后不进入慢启动
net.ipv4.tcp_slow_start_after_idle = 0

#允许最大重排数量
net.ipv4.tcp_reordering = 8

```
```
#拥塞算法
net.ipv4.tcp_available_congestion_control = cubic reno bbr
net.ipv4.tcp_congestion_control = bbr
```
```
#重传策略
net.ipv4.tcp_sack = 1
net.ipv4.tcp_fack = 1
net.ipv4.tcp_dsack = 1
```

#### 连接回收
```
net.ipv4.tcp_tw_reuse       = 1
net.ipv4.tcp_tw_recycle     = 1
net.ipv4.tcp_timestamps     = 0

net.ipv4.tcp_fin_timeout    = 2
net.ipv4.tcp_max_tw_buckets = 6000
```
