#define _USE_MATH_DEFINES
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

#include "apperrs.hpp"

Message::Message( 
    string _a, int _e, string _m, string _f, string _c, int _l 
  ) : 
  appname  (_a),
  errnum   (_e),
  message  (_m),
  filename (_f),
  funcname (_c),
  lineno   (_l)
{

}

string Message::print_oneline( void ) {
  ostringstream out;
  out << appname << ":" << errnum;
  out << " F:" << filename;
  out << " N:" << funcname;
  out << " L:" << lineno;
  out << " M:" << message;
  out << endl; 
  return out.str();
}

string Message::print_long( void ) {
  ostringstream out;
  out << "APP:   " << appname  << endl;
  out << "ERRNO: " << errnum    << endl;
  out << "ERROR: " << message  << endl;
  out << "FILE:  " << filename << endl;
  out << "FUNC:  " << funcname << endl;
  out << "LINE:  " << lineno   << endl;
  return out.str();
}

    
Apperrs::Apperrs(void) {
  apperrs.clear();
}

int Apperrs::push( string appname, int errnum, string message, 
                    string filename, string funcname, int lineno ) {
   apperrs.push_back( Message( appname, errnum, message, filename, funcname, lineno ) );
   return errnum;
}

Message Apperrs::pop( void ) {
  Message m;
  m = apperrs.back();
  apperrs.pop_back();
  return m;
}

bool Apperrs::all_okay( void ) { 
  return apperrs.size() == 0;
}

string Apperrs::print_all( void ) {
  ostringstream out;
  while( apperrs.size() != 0 ) { // suck the stack dry
    out << apperrs.back().print_oneline();
    apperrs.pop_back();
  }
  return out.str();
}


