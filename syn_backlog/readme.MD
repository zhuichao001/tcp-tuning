The maximum length of the queue for incomplete sockets can be set using /proc/sys/net/ipv4/tcp_max_syn_backlog. When syncookies are enabled there is no logical maximum length and this setting is ignored.

max sync-recv queue size: roundup_pow_of_two(backlog, somaxconn, tcp_max_syn_backlog)
