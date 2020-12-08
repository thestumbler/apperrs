#ifndef _METER_HPP
#define _METER_HPP

#include <cstdlib>
#include <string>
#include <iostream>
#include "apperrs.hpp"
using namespace std;

//#define DEFINE_MANUALLY
#define DEFINE_USING_MACRO_TABLE
// #define NO_ERROR_STRINGS

class Meter {
  private:
  public:

    //===APPERR BEG===========================================================
#ifdef DEFINE_USING_MACRO_TABLE
    //   error number, error string
    #define METER_ERRORS(F) \
      F( no_input, "No input signal" ) \
      F( over_current, "Current too large" ) \
      F( over_voltage, "Over voltage" ) \
      F( low_battery, "Low battery" ) \
      F( lead_fault, "Test lead fault" ) \
      F( pegged, "Needle is pegged" ) \
      F( range_under, "Measured valus is under-range" ) \
      F( range_over, "Measured valus is over-range" ) \
    /**/ 
    #define EXTRACT_ENUMS( ID, STRING ) ID,
    enum class Eid { 
      okay=0,
      METER_ERRORS(EXTRACT_ENUMS)
      size
    };
    #undef EXTRACT_ENUMS
    #define EXTRACT_DEFINES( ID, STRING )  STRING,
    const string errors[(int)Eid::size]={
      "Okay",
      METER_ERRORS(EXTRACT_DEFINES)
    };
    #undef EXTRACT_DEFINES
#endif

#ifdef DEFINE_MANUALLY
    enum class Eid {
      okay=0,
      no_input,
      over_current,
      over_voltage,
      low_battery,
      lead_fault,
      pegged,
      range_under,
      range_over,
      size
    };
    const string errors[(int)Eid::size]={
     "Okay",
     "No input signal",
     "Current too large",
     "Over voltage",
     "Low battery",
     "Test lead fault",
     "Needle is pegged",
     "Measured value is under-range",
     "Measured value is over-range",
    };
#endif
    //===APPERR END===========================================================




    Apperrs& apperrs;

    Meter(Apperrs& _apperrs);
    int test( int errs[], int numerrs );
};

//#undef DEFINE_MANUALLY
//#undef DEFINE_USING_MACRO_TABLE
//#undef NO_ERROR_STRINGS

#endif
