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
