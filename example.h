#ifndef _EXAMPLE_H
#define _EXAMPLE_H

//===APPERR BEG===========================================================
//   name, error_string
#define METER_ERRORS(F) \
  F( METER_NO_INPUT, "No input signal", ) \
  F( METER_OVER_CURRENT, "Current too large", ) \
  F( METER_OVER_VOLTAGE, "Over voltage", ) \
  F( METER_LOW_BATTERY, "Low battery", ) \
  F( METER_LEAD_FAULT, "Test lead fault", ) \
  F( METER_PEGGED, "Needle is pegged", ) \
  F( METER_RANGE_UNDER, "Measured valus is under-range", ) \
  F( METER_RANGE_OVER, "Measured valus is over-range", ) \
/**/ 
#define EXTRACT_ENUM( NAME, STRING ) NAME,
enum tagMETER_ERRORS {
  METER_OKAY=0,
  METER_ERRORS(EXTRACT_ENUM)
  METER_NUM_ERRORS,
};
#undef EXTRACT_ENUM
//===APPERR END===========================================================


#define APPERR_PUSH(A) apperr_push("METER", A, (char *)METER_ERROR_STRINGS[A], (char *)__FILE__, (char *)__FUNCTION__, __LINE__ )



int meter_test( int *errs, int numerrs ) {
  int i;
  apperr_return_clear();
  for(i=0;i<numerrs;i++) {
    switch(errs[i]) {
      case 1:
        APPERR_PUSH( METER_NO_INPUT );
        break;
      case 2:
        APPERR_PUSH( METER_OVER_CURRENT );
        break;
      case 3:
        APPERR_PUSH( METER_OVER_VOLTAGE );
        break;
      case 4:
        APPERR_PUSH( METER_LOW_BATTERY );
        break;
      case 5:
        APPERR_PUSH( METER_LEAD_FAULT );
        break;
      case 6:
        APPERR_PUSH( METER_PEGGED );
        break;
      case 7:
        APPERR_PUSH( METER_RANGE_UNDER );
        break;
      case 8:
        APPERR_PUSH( METER_RANGE_OVER );
        break;
      case 0:
      default:
        APPERR_PUSH( METER_OKAY );
        break;
    }
  }
  return apperr_return_int();
}

int main( int argc, char *argv[] ) {
  int iret;
  int i,j,n;
  int test_errs[64];
  APPERR *pe;
  for(i=1;i<64;i++) test_errs[i]=0;

  printf("========================================================================\n");
  printf("                          APPERR functional test\n");
  printf("========================================================================\n");


  fprintf(stderr, "---------------\n");
  fprintf(stderr, "test function will not push anything on the apperr stack\n");
  fprintf(stderr, "this would be a typical example in the case of no error\n");
  iret=meter_test( test_errs, 0 );
  fprintf(stderr, "return code: %d\n", iret );
  apperr_print_all(stderr);

  fprintf(stderr, "---------------\n");
  fprintf(stderr, "now the func will push OKAY status on the apperr stack\n");
  fprintf(stderr, "it is another way, pro-active, to indicate function success\n");
  test_errs[0]=0;
  iret=meter_test( test_errs, 1 );
  fprintf(stderr, "return code: %d\n", iret );
  apperr_print_all(stderr);

  // now test each error singly
  for(j=1;j<METER_NUM_ERRORS;j++) {
    fprintf(stderr, "---------------\n");
    fprintf(stderr, "now the func will push one error on the apperr stack: \n");
    fprintf(stderr, "errno %d (%s)\n", j, METER_ERROR_STRINGS[j] );
    test_errs[0]=j;
    iret=meter_test( test_errs, 1 );
    fprintf(stderr, "return code: %d\n", iret );
    apperr_print_all(stderr);
  }

  // now test them in multiples
  for(n=2;n<=10;n++) {
  for(j=1;j<32;j++) {
    fprintf(stderr, "---------------\n");
    fprintf(stderr, "now the func will push %d errors on the apperr stack: \n", n );
    for(i=0;i<n;i++) {
      test_errs[i]=1+(rand() % (METER_NUM_ERRORS-1));
      fprintf(stderr, "errno %d (%s)\n", test_errs[i], METER_ERROR_STRINGS[test_errs[i]] );
    }
    iret=meter_test( test_errs, n );
    fprintf(stderr, "return code: %d\n", iret );
    apperr_print_all(stderr);
  }
  }

}

#endif // _EXAMPLE_H
