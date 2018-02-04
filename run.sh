#!/bin/bash
if [ ! -d "build" ];then
    mkdir "build"
fi

cd build
cmake ..
make install
make -j8
./FacelessPerson