#!/bin/bash
set -Cu

script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir

sudo rm /dev/myled0
sudo rmmod led
make clean
