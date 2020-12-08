g++ -std=c++11 -Iinc -c src/apperrs.cpp -o build/apperrs.o
ar rcs lib/libapperrs.a build/apperrs.o
