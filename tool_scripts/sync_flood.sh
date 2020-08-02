#! /bin/bash


TARGET=127.0.0.1
sudo hping3 -c 1000 -d 120 -w 64 --rand-source $TARGET -S -q -p 8723 --flood
