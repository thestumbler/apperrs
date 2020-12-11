#ifndef _BALOON_HPP
#define _BALOON_HPP

#include <cstdlib>
#include <string>
#include <iostream>
#include "apperrs.hpp"
#include "meter.hpp"
using namespace std;

// #define NO_ERROR_STRINGS

class Balloon {
  private:
  public:

    //===APPERR BEG===========================================================
    //   error number, error string
    #define BALLOON_ERRORS(F) \
      F( gas_helium, "filled with Helium" ) \
      F( gas_air, "filled with air" ) \
      F( gas_hydrogen, "danger, filled with hydrogen" ) \
      F( over_inflation, "overinflation detected" ) \
      F( under_inflation, "underinflation detected" ) \
      F( knot_loose, "knot is coming loose" ) \
      F( burst_soon, "burst imminent" ) \
      F( burst_happened, "is bursted" ) \
    /**/ 
    #define EXTRACT_ENUMS( VARNAME, STRING ) VARNAME,
    enum class Eid { 
      okay=0,
      BALLOON_ERRORS(EXTRACT_ENUMS)
      size
    };
    #undef EXTRACT_ENUMS
    #define EXTRACT_DEFINES( ID, STRING )  STRING,
    const string errors[(int)Eid::size]={
      "Okay",
      BALLOON_ERRORS(EXTRACT_DEFINES)
    };
    #undef EXTRACT_DEFINES
    //===APPERR END===========================================================

    Apperrs& apperrs;
    Balloon(Apperrs& _apperrs);
    int test( int errs[], int numerrs );
    int nested( Meter& meter );
    int nested2( Meter& meter );
};

#undef DEFINE_MANUALLY
#undef DEFINE_USING_MACRO_TABLE
#undef NO_ERROR_STRINGS

#endif
