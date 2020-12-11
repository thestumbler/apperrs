#!/bin/bash
#g++ -std=c++11 -Iinc -c src/apperrs.cpp -o build/apperrs.o
clang++ -std=c++11 -Iinc -c src/apperrs.cpp -o build/apperrs.o
#ar rcs lib/libapperrs.a build/apperrs.o
libtool -static -o lib/libapperrs.a build/apperrs.o
cp ./lib/libapperrs.a /usr/local/lib
cp ./inc/apperrs.hpp /usr/local/include
