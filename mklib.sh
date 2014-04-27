gcc -c apperr.c
ar rcs libapperr.a apperr.o
sudo cp libapperr.a /usr/local/lib
sudo cp apperr.h /usr/local/include
gcc test.c -lapperr -o test
