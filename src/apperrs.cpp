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

string Message::print_oneline( string prefix ) {
  ostringstream out;
  out << prefix;
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
  queue.clear();
}

int Apperrs::push( string appname, int errnum, string message, 
                    string filename, string funcname, int lineno ) {
   queue.push_back( Message( appname, errnum, message, filename, funcname, lineno ) );
   return errnum;
}

Message Apperrs::pop( void ) {
  Message m;
  m = queue.back();
  queue.pop_back();
  return m;
}

bool Apperrs::all_okay( void ) { 
  return queue.size() == 0;
}

string Apperrs::print_all( string prefix ) {
  ostringstream out;
  while( queue.size() != 0 ) { // suck the stack dry
    out << queue.back().print_oneline(prefix);
    queue.pop_back();
  }
  return out.str();
}

string Apperrs::print_messages( string prefix ) {
  ostringstream out;
  for(auto msg : queue) {
    if( msg.errnum != 0 ) continue;
    if( msg.message == "okay" ) continue;
    out << msg.print_oneline(prefix);
  }
  return out.str();
}

void Apperrs::clear( void ) {
  queue.clear();
}

