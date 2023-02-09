## 建连
```
net.core.somaxconn = 65536
监听端口的最大全连接长度
默认值是128
如果过小，压力较大的时候会拒绝连接
```

```
net.ipv4.tcp_max_syn_backlog = 65536
能接受SYN同步包的最大客户端数量，即半连接上限
默认值是128
如果过小，客户端sync后无法收到ack，也就是连接超时
```

```
net.ipv4.tcp_abort_on_overflow = 1
当最大全连接队列溢出时，发送RST关闭连接
默认值是0
如果默认是0，队列溢出时无声无息的丢弃所收到的包，这样会导致客户端不会重连
```

```
net.ipv4.tcp_syncookies = 0
当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击
默认为0，表示关闭
如果关闭，不能预防SYN FLOOD
```

```
net.core.netdev_max_backlog = 102400
已连接但是内核还没有通知应用层接收的最大连接数
默认值缺失
如果过小，在极端压力下会来不及Accept
```

```
net.ipv4.tcp_syn_retries = 2
对于本地发起建联的请求，需要最大尝试的SYN次数，超过最大次数则放弃
默认值为5
过大则导致发起建连等待时间过长
```

```
net.ipv4.tcp_synack_retries = 2
收到对端的SNC包后发送SYNC+ACK的最大次数，超过此数则放弃连接
默认值为5
如果过大，则可能导致半连接队列被耗尽
```

```
net.ipv4.ip_local_port_range = 1024 65535
本地可用端口的范围
默认值是  21000   61000
如果范围太小，对外连接数会受到限制
```

## 缓冲区相关
```
net.ipv4.tcp_window_scaling = 1
滑动窗口是否支持通过扩大因子来加大
默认是0
如果滑动窗口大于64k，需要打开此开关
```

```
net.core.optmem_max = 10240000
每个套接字所允许的最大缓冲区的大小
默认是20480
增大到10m有好处是对于大包支持的更好一些，但是总体发送缓冲需要应用层座一层保证
```

```
net.core.rmem_default = 8388608
每个套接字默认读缓冲大小
会在net.ipv4.tcp_rmem中体现

net.core.wmem_default = 8388608
每个套接字默认写缓冲大小
会在net.ipv4.tcp_wmem中体现

net.core.rmem_max = 16059200
每个套接字最大读缓冲大小
会在net.ipv4.tcp_rmem中体现

net.core.wmem_max = 16059200
每个套接字最大写缓冲大小
会在net.ipv4.tcp_wmem中体现
```

```
net.ipv4.tcp_mem = 7864320 10485760 15728640
TCP 栈应该如何反映内存使用，单位是页（4k）
第一个值是内存使用的下限。
第二个值是内存压力模式开始对缓冲区使用应用压力的上限。
第三个值是内存上限。在这个层次上可以将报文丢弃，从而减少对内存的使用。

net.ipv4.tcp_rmem = 786432 2097152 31457280
为自动调优每个套接字的读缓冲大小，单位字节
第一个值是为 socket 的接收缓冲区分配的最少字节数。
第二个值是默认值（该值会被 rmem_default 覆盖），缓冲区在系统负载不重的情况下可以增长到这个值。
第三个值是接收缓冲区空间的最大字节数（该值会被 rmem_max 覆盖）。

net.ipv4.tcp_wmem = 786432 2097152 31457280
为自动调优每个套接字的写缓冲大小，单位字节
第一个值是为 socket 的发送缓冲区分配的最少字节数。
第二个值是默认值（该值会被 wmem_default 覆盖），缓冲区在系统负载不重的情况下可以增长到这个值。
第三个值是发送缓冲区空间的最大字节数（该值会被 wmem_max 覆盖）。
```

## 流控&拥塞控制
```
net.ipv4.tcp_sack = 1
有选择的应答开关
默认1
通过有选择地应答乱序接收到的报文来提高性能
```

```
net.ipv4.tcp_dsack = 1
是否允许重复发送选择重传
默认1
让发送方知道具体的丢包原因，更好的流控
```

```
net.ipv4.tcp_fack = 1
快速重传开关
默认1
如果tcp_sack为0，这个开关即便打开是无效的
```

```
net.ipv4.tcp_slow_start_after_idle = 0
关闭tcp的连接传输的慢启动一段时间后，再初始化拥塞窗口
默认0
还是从慢启动开始相对来说更靠谱
```

```
net.ipv4.tcp_congestion_control = bbr  
默认reno
建议如果不支持bbr，至少可以升级到cubic算法
```

## 回收释放
```
net.ipv4.tcp_timestamps = 1
开启TCP时间戳
默认0
可以防范伪造的seq
```

```
net.ipv4.tcp_tw_reuse = 1
表示是否允许重新应用处于TIME-WAIT状态的socket用于新的TCP连接
默认值是0
打开可以快速重用连接
```

```
net.ipv4.tcp_tw_recycle = 1 #慎用
开启TCP连接中TIME-WAIT sockets的快速回收
默认值是0
打开可以快速回收连接
```

```
net.ipv4.tcp_fin_timeout = 10
TCP连接保持在FIN-WAIT-2状态的时间， 即MSL，单位秒
默认值是60秒
改小此值可以让主动关闭方在合理的时间内尽快回收套接字
```

```
net.ipv4.tcp_max_tw_buckets = 10240
允许同时保持TIME_WAIT状态套接字的最大数量，超过次数则直接回收
默认值180000
不要让过多套接字处于此状态，以免无法建连
```


## 连接保活
```
net.ipv4.tcp_keepalive_time = 100
tcp发起探测包的周期，单位秒
默认值7200秒
服务端可以主动探测连接是否有效，及早关闭死连接
```

```
net.ipv4.tcp_keepalive_probes = 3
tcp发起探测包的最大重试次数，超过最大次数，则主动关闭连接
默认值为9
快速回收无效连接
```

```
net.ipv4.tcp_keepalive_intvl = 10
tcp发起探测包未得到相应的重试间隔时间，单位秒
默认值为75
尽快重试，以便快速回收无效连接
```

## 重试
```
net.ipv4.tcp_syn_retries = 2
在内核放弃建立连接之前发送SYN包的数量
默认为5
减小建连的等待时间，失败则尽快交给应用层决定是否重连
```

```
net.ipv4.tcp_retries1 = 3
放弃回应一个tcp连接请求的最大次数
默认3，TCP规定最小为3
过大则导致对于网络较差的情况下加大网络负担，同时释放死连接也较慢
```

```
net.ipv4.tcp_retries2 = 4
在丢弃激活(已建立通讯状况)的TCP连接之前﹐需要进行多少次重试
默认为15
适当的改小有助于弱网络下减小网络压力，快速释放连接
```