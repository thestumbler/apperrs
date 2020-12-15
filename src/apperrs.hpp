#ifndef _APPERR_HPP
#define _APPERR_HPP

#include <string>
#include <vector>
using namespace std;

class Message {
  private:
  public:
    int errnum;       // application error number
    string appname;    // name of the application
    string message;    // error text
    string filename;   // file of error
    string funcname;   // function name
    int lineno;      // line number

    Message( string _a="", int _e=0, string _m="", 
             string _f="", string _c="", int _l=0 );
    string print_oneline( string prefix = "" );
    string print_long( void );
    string print( void );
    bool is_okay( void );
};

class Apperrs { // vector of Apperr
  private:
  public:

    struct Defines {
      int id;
      string message;
    };

    vector<Message> queue;
    Apperrs();
  
    int push( string appname, int errnum, string message, 
               string filename, string funcname, int lineno );
    Message pop( void );
    bool all_okay( void );
    string print_all( string prefix = "" );
    string print_messages( string prefix = "" );
    void clear( void );

};

#endif // _APPERR_HPP
