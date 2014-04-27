cl -c apperr.c
lib apperr.obj -OUT:apperr.lib
cl test.c apperr.lib
