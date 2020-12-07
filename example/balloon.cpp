#include <cstdlib>
#include <string>
#include <iostream>
#include "balloon.hpp"
#include "meter.hpp"
using namespace std;

Balloon::Balloon( Apperrs& _apperrs)
  : apperrs(_apperrs)
{

    #ifdef NO_ERROR_STRINGS
    #define APPERR_PUSH(A) apperrs.push("Balloon", (int)A, "", __FILE__, string(__FUNCTION__), __LINE__ )
    #else
    #define APPERR_PUSH(A) apperrs.push("Balloon", (int)A, errors[(int)A], __FILE__, string(__FUNCTION__), __LINE__ )
    #endif
}

int Balloon::test( int errs[], int numerrs ) {
  cout << "Number errors: " << numerrs << endl;
  for(int i=0;i<numerrs;i++) {
    switch(errs[i]) {
      case 1:
        return APPERR_PUSH( Eid::gas_helium );
        break;
      case 2:
        return APPERR_PUSH( Eid::gas_air );
        break;
      case 3:
        return APPERR_PUSH( Eid::gas_hydrogen );
        break;
      case 4:
        return APPERR_PUSH( Eid::over_inflation );
        break;
      case 5:
        return APPERR_PUSH( Eid::under_inflation );
        break;
      case 6:
        return APPERR_PUSH( Eid::knot_loose );
        break;
      case 7:
        return APPERR_PUSH( Eid::burst_soon );
        break;
      case 8:
        return APPERR_PUSH( Eid::burst_happened );
        break;
      case 0:
      default:
        break;
    }
  }
  int iret = APPERR_PUSH( Eid::okay );           
  return iret;
}

int Balloon::nested( Meter& meter ) {
  int errs[1] = { (int)Meter::Eid::low_battery };
  meter.test( errs, 1 );
  return APPERR_PUSH( Eid::burst_soon );
}

int Balloon::nested2( Meter& meter ) {
  int errs[1] = { (int)Meter::Eid::low_battery };
  meter.test( errs, 1 );
  return APPERR_PUSH( Eid::okay );
}

#undef APPERR_PUSH
