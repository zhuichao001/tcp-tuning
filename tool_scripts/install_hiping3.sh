yum install libpcap
yum install libpcap-devel
ln -sf /usr/include/pcap-bpf.h /usr/include/net/bpf.h
yum -y install tcl-devel

git clone https://github.com/antirez/hping
./configure
make
sudo su
make install
