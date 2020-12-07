#include <cstdlib>
#include <string>
#include <iostream>
#include "meter.hpp"
using namespace std;


Meter::Meter( Apperrs& _apperrs)
  : apperrs(_apperrs)
{
    #ifdef NO_ERROR_STRINGS
    #define APPERR_PUSH(A) apperrs.push("Meter", (int)A, "", __FILE__, string(__FUNCTION__), __LINE__ )
    #else
    #define APPERR_PUSH(A) apperrs.push("Meter", (int)A, errors[(int)A], __FILE__, string(__FUNCTION__), __LINE__ )
    #endif
}

int Meter::test( int errs[], int numerrs ) {
  cout << "Number errors: " << numerrs << endl;
  for(int i=0;i<numerrs;i++) {
    switch(errs[i]) {
      case 1:
        return APPERR_PUSH( Eid::no_input );
        break;
      case 2:
        return APPERR_PUSH( Eid::over_current );
        break;
      case 3:
        return APPERR_PUSH( Eid::over_voltage );
        break;
      case 4:
        return APPERR_PUSH( Eid::low_battery );
        break;
      case 5:
        return APPERR_PUSH( Eid::lead_fault );
        break;
      case 6:
        return APPERR_PUSH( Eid::pegged );
        break;
      case 7:
        return APPERR_PUSH( Eid::range_under );
        break;
      case 8:
        return APPERR_PUSH( Eid::range_over );
        break;
      case 0:
      default:
        int iret = APPERR_PUSH( Eid::okay );           
        return iret;
        break;
    }
  }
  return 0;
}
#undef APPERR_PUSH
