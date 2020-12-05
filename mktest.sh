gcc -c apperr.c
ar rcs libapperr.a apperr.o
gcc test.c libapperr.a -o test

