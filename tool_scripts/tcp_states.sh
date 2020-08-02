#! /bin/bash
set -x

netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
