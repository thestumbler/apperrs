#ifndef _APPERR_H
#define _APPERR_H

#include <stdio.h>

#ifdef _WIN32
// stdint.h for Microsoft Visual C, obtained from:
// http://code.google.com/p/msinttypes/
#include "stdint_win.h"
// stdint.h for Microsoft Visual C, obtained from:
// http://waterjuice.org/2013/06/stdbool-h/
#include "stdbool_win.h"
#else
#include <stdint.h>
#include <stdbool.h>
#endif

typedef struct tagAPPERR {
  char *app;     // name of the application
  int num;       // application errno
  char *str;    // error text
  char *file;
  char *func;
  int line;
} APPERR;

void apperr_init( void );
void apperr_push( char *app, int num, char *text, 
                  char *file, char *func, int line );
extern APPERR *apperr_pop( void );
extern bool apperr_print( FILE *fp, APPERR *pae );
extern bool apperr_print_all( FILE *fp );
extern bool apperr_okay( void );

#endif // _APPERR_H
