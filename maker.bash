#!/bin/bash
set -Ceu

script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir

make
sudo insmod led.ko
sudo chmod 666 /dev/myled0
ls -l /dev/myled0
