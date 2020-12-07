#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

#include "apperrs.hpp"
#include "meter.hpp"
#include "balloon.hpp"


Apperrs apperrs;
Meter meter(apperrs);
Balloon balloon(apperrs);

int main( int argc, char *argv[] ) {
  int iret;
  int test_errs[64];

  cout << "========================================================================" << endl;
  cout << "                          APPERR functional test" << endl;
  cout << "========================================================================" << endl;


  // Test: nothing on the error stack
  cout << "---------------" << endl;
  cout << "test function will not push anything on the apperr stack" << endl;
  cout << "this would be a typical example in the case of no error" << endl;
  iret = meter.test( test_errs, 0 );
  cout << "return code: " << iret << endl;
  cout << apperrs.print_all();

  // Test: okay on the error stack
  cout << "---------------" << endl;
  cout << "now the func will push OKAY status on the apperr stack" << endl;
  cout << "it is another way, pro-active, to indicate function success" << endl;
  test_errs[0]=0;
  iret=meter.test( test_errs, 1 );
  cout << "function called, return code: " << iret << endl;
  cout << apperrs.print_all();

  // Meter, Test: each error singly
  for(int j=1;j<(int)Meter::Eid::size;j++) {
    cout << "---------------" << endl;
    cout << "now the func will push one error on the apperr stack: " << endl;
    cout << "  errno " << j << ", " << meter.errors[j] << endl;
    test_errs[0]=j;
    iret=meter.test( test_errs, 1 );
    cout << "function called, return code: " << iret << endl;
    cout << apperrs.print_all();
  }

  // Balloon, Test: each error singly
  for(int j=1;j<(int)Balloon::Eid::size;j++) {
    cout << "---------------" << endl;
    cout << "now the func will push one error on the apperr stack: " << endl;
    cout << "  errno " << j << ", " << balloon.errors[j] << endl;
    test_errs[0]=j;
    iret=balloon.test( test_errs, 1 );
    cout << "function called, return code: " << iret << endl;
    cout << apperrs.print_all();
  }

#if 0
  // Meter & Balloon Test: each error singly, both functions
  for(int j=1;j<(int)Meter::Eid::size;j++) {
    cout << "---------------" << endl;
    cout << "now the func will push one error on the apperr stack: " << endl;
    cout << "  errno " << j << ", " << meter.errors[j] << endl;
    test_errs[0]=j;
    iret=meter.test( test_errs, 1 );
    cout << "function called, return code: " << iret << endl;
  }
  for(int j=1;j<(int)Balloon::Eid::size;j++) {
    cout << "---------------" << endl;
    cout << "now the func will push one error on the apperr stack: " << endl;
    cout << "  errno " << j << ", " << balloon.errors[j] << endl;
    test_errs[0]=j;
    iret=balloon.test( test_errs, 1 );
    cout << "function called, return code: " << iret << endl;
  }
  cout << apperrs.print_all();
#endif

  // Meter & Balloon Nested Test:
  cout << "---------------" << endl;
  cout << "testing nested error returns on the apperr stack: " << endl;
  iret=balloon.nested(meter);
  cout << "function called, return code: " << iret << endl;
  cout << apperrs.print_all();

  // Meter & Balloon Nested Test:
  cout << "---------------" << endl;
  cout << "testing nested error returns on the apperr stack: " << endl;
  iret=balloon.nested2(meter);
  cout << "function called, return code: " << iret << endl;
  cout << apperrs.print_all();

#if 0
  // Test: multiple combination of errors
  for(int n=2; n<=10; n++) {
    for(int j=1; j<32; j++) {
      cout << "---------------" << endl;
      cout << "now the func will push " << n << " errors on the apperr stack:" << endl;
      for(int i=0; i<n; i++) {
        test_errs[i]=1+(rand() % ((int)Meter::Eid::size-1));
        cout << "  errno " << test_errs[i] << ", " << meter.errors[test_errs[i]] << endl;
      }
      iret=meter.test( test_errs, n );
      cout << "function called, return code: " << iret << endl;
      cout << apperrs.print_all();
    }
  }
#endif

}
