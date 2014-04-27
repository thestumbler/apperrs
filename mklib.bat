cl -c apperr.c
lib apperr.obj -OUT:apperr.lib
copy apperr.lib c:\lib
copy apperr.h c:\inc
