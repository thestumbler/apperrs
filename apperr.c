#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef _WIN32
// stdint.h for Microsoft Visual C, obtained from:
// http://code.google.com/p/msinttypes/
#include "stdint_win.h"
// stdint.h for Microsoft Visual C, obtained from:
// http://waterjuice.org/2013/06/stdbool-h/
#include "stdbool_win.h"
#include "windows.h"
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#include "apperr.h"

//*****************************************************************
///  Application Error Handling 
///
///  Previous
/// (1) DEBUG mode error reporting (non-production?)
///
///    NXP library uses an interesting error reporting method for
///    parameter checking.  It works like this:
///
///    (a) macro to check the validity of a parameter, e.g., 
///        #define PARAM_HAIR_LENGTH(A) ((A>=0)&&(A<=10))
///
///    (b) this is utilized within another macro, CHECK_PARAM:
///        #define CHECK(expr) \
///        ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
///
///  (c) this would be invoked inline a function as follows:
///      CHECK(PARAM_HAIR_LENGTH(A))
///
///  Should the check fail, the function check_failed() is called.
///  This example function stops the program, after printing the
///  filen and line number.
///
///    void check_failed(uint8_t *file, uint32_t line) {
///      // User can add his own implementation to report 
///      // the file name and line number
///      printf("Wrong parameters value: file %s on line %d\r\n", file, line);
///      printf("Rebooting\r\n");
///      NVIC_SystemReset();
///      while(1);
///    }
///
///  This method is optionally compiled in / out under control 
///  of a #ifdef DEBUG compile-time switch, so presumably such
///  checking wouldn't be delivered in production code.
///
///  However, a suitable check_failed() function could be writting 
///  that did something more tolerable that stopping the processor,
///  mabybe log the error, alert the user, or report an error up-stream
///  to another controller. 
///
///  Although not used in the NXP examples, many compilers also 
///  support the __FUNCTION__ macro, which may not be standard.
///  __func__ is defined by C99.
///
/// (2) ERRNO-like method
///
///   Functions return error codes to the caller, typically using
///   the convention that 0 means okay.  If an error is detected,
///   I've seen two methods used:
/// 
///   (a) negative error number.  This can be used when the return
///   value from a function is a signed integer, and the normal
///   (non-error) values are always positive.  This makes it easy for
///   the caller to detect an error, such as:
///
///       iret=getNumBytesRead();
///       if(iret<0) {
///         // error handling code
///       } else {
///         // normal processing
///       }
///
///   (b) an error number is passed outside the calling mechanism, 
///   via a global errno variable, such as is used by the C-standard
///   in errno.h.  Because this is separate from the normal function
///   returned parameter, usually these numbers are positive.
/// 
/// (3) Error ID Strings
/// 
///   The error number can be used to access a string defining the
///   type of error.  If the errnos are sequential beginning at zero,
///   this is simply an index into an array of string pointers.  If
///   we allow error numbers to be negative, or offset from zero, then
///   some processing has to be done to translate errno into string array
///   index.  The C-library provides perror() and strerror() functions
///   to access these strings.
/// 
/// Conclusion, This Application Library Error Reporting Method
///
///    Let's take an errno-like approach, and also use the __FILE__, 
///    __LINE__ and __FUNCTION__ macros.  If desired, these can be
///    compiled out for production code.
///
///    Furthermore, define the concept of a local (within a single
///    function) error number. This number can be used as a return
///    integer, which can be used primarily as a true-false indicator
///    by the calling function.
/// 
/// Issues
/// 
/// (a) 
/// 
/// @file __FILE__
/// @author Chris Lott
/// @version 1.0
///
//*****************************************************************
    

#define APPERR_MAX_STACK (64)
static APPERR ae[APPERR_MAX_STACK];
static int ptr=0;

void apperr_init( void ) {
  for( ptr=0; ptr<APPERR_MAX_STACK; ptr++) {
    ae[ptr].app = 0;
    ae[ptr].num = 0;
    ae[ptr].str = NULL;
    ae[ptr].file = NULL;
    ae[ptr].func = NULL;
    ae[ptr].line = 0;
  }
  ptr=0;
}

void apperr_push( char *app, int num, char *str, 
                  char *file, char *func, int line ) {
  ae[ptr].app = app;
  ae[ptr].num = num;
  ae[ptr].str = str;
  ae[ptr].file = file;
  ae[ptr].func = func;
  ae[ptr].line = line;
  if(ptr<APPERR_MAX_STACK) ptr++;
}

bool apperr_okay( void ) { 
  if(ptr==0) return true; // if nothing on the stack, no error
  return ae[ptr-1].num == 0;
}

APPERR *apperr_pop( void ) {
  if(ptr==0) return NULL;
  ptr--;
  return ae+ptr;
}

#define APPERR_TWO_LINE
bool apperr_print( FILE *fp, APPERR *pae ) {
  bool bret=false;
  if(!pae) return bret;
  if(pae->num!=0) bret=true;
#ifdef APPERR_TWO_LINE
  fprintf(fp, "%s: %d, %s\n", pae->app, pae->num, pae->str );
  fprintf(fp, "FILE:%s: FUNC:%s, LINE:%d\n", pae->file, pae->func, pae->line );
#else
  fprintf(fp, "APP: %s, ", pae->app );
  fprintf(fp, "ERRNO: %3d, ", pae->num );
  fprintf(fp, "ERROR: %s, ", pae->str );
  fprintf(fp, "FILE: %s, ", pae->file );
  fprintf(fp, "FUNC: %s, ", pae->func );
  fprintf(fp, "LINE: %4d, ", pae->line );
  fprintf(fp, "\n");
#endif
  return bret;
}

bool apperr_print_all( FILE *fp ) {
  APPERR *pe;
  bool bret=false;
  do { // suck the stack dry
    pe=apperr_pop();
    if(apperr_print(fp,pe)) bret=true;
  } while(pe);
  return bret;
}
