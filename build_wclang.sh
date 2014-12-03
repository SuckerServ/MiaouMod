#!/bin/bash

git clone https://github.com/tpoechtrager/wclang.git
cd wclang
sed "s|-rfs \$(DESTDIR)|-fs /|g" -i src/Makefile.am
./bootstrap.sh
./configure --prefix=/usr
make
sudo make install
