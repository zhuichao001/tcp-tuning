#! /bin/bash
set -x

iptables -A INPUT -s 173.0.0.0/8 -p tcp  –dport 80 -j DROP
