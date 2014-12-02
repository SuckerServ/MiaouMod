#!/bin/bash

git clone https://github.com/tpoechtrager/wclang.git
cd wclang
./bootstrap.sh
./configure --prefix=/usr
make
sudo make install
