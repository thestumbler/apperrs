g++ -std=c++11 -c -I./src ./example/errtest.cpp  -o ./build/errtest.o
g++ -std=c++11 -c -I./src ./example/balloon.cpp  -o ./build/balloon.o
g++ -std=c++11 -c -I./src ./example/meter.cpp  -o ./build/meter.o
g++ -std=c++11 ./build/errtest.o ./build/meter.o ./build/balloon.o \
  -lapperrs -o ./bin/errtest

