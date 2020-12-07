# Application Error Handling Module APPERRS

## Usage, error table preparation

A table of errors codes and strings must be prepared.
You could make them program-wide, but I prefer to do it by module.
There are at least two ways to do it, see example file `meter.hpp`.
* Manual
* Macro tables

Arbitrary error numbers, instead of sequential ones, or negative error
numbers could be obtained by adding another column to the table, and
adding some functions to index the text based on the error number.  In
C++ a mapping could be made. 

## Usage, calling MACRO preparation

A calling macro must be prepared, corresponding with the variables used to
keep the error numbers and text.  For example, from `test.cpp`:

* Error numbers are in an enum class `Module::Eid::error_identifier` 
* Error text are in a string array called `module.errors[]`

A macro should be prepared in the module's constructor:

```c
#define APPERR_PUSH(A) \
apperrs.push("Meter", (int)A, errors[(int)A], \
__FILE__, string(__FUNCTION__), __LINE__ )
```

This macro can be compiled to return full data, errno and debugging
info, or just errno.

## Usage, in action

Once the table is defined and invocation macro prepared, the an error
can be popped on the stack like this:

```c
return APPERR_PUSH( Eid::over_voltage );
```

The calling program can unwind the errors as follows:

```c
  int iret = some_function( );
  if( !apperrs.all_okay() ) {
    cout << apperrs.print_all();
  }
```




## Background

I was struggling with reporting meaningful error messages in both
embedded systems and small PC programs which communicate with them.
I since learned that this is a long-standing programming problem, with
various solutions and no universal agreement on the "correct" approach.

### Previous Methods

For my applications, I first took a look at the error handling methods
available in programs I was already working on.

#### DEBUG mode error reporting (non-production?)

In the NXP sample code libraries I was using, they used an
interesting error reporting method for parameter checking.  It works
like this:

* macro to check the validity of a parameter, e.g., 

```c
#define PARAM_HAIR_LENGTH(A) ((A>=0)&&(A<=10))
```

* this is utilized within another macro, CHECK:

```c
#define CHECK(expr) \
((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
```

* this would be invoked inline a function as follows:

```c
CHECK(PARAM_HAIR_LENGTH(A))
```

Should the check fail, the function `check_failed()` is called.
This example function stops the program, after printing the
file and line number.

```c
void check_failed(uint8_t *file, uint32_t line) {
  // User can add his own implementation to report 
  // the file name and line number
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  printf("Rebooting\r\n");
  NVIC_SystemReset();
  while(1);
}
```
This method is optionally compiled in / out under control 
of an `#ifdef DEBUG` compile-time switch, so presumably such
checking wouldn't be delivered in production code.

However, a suitable `check_failed()` function could be written 
that did something more tolerable than stopping the processor.
Maybe log the error, alert the user, or report an error up-stream
to another controller. 

Although not used in the NXP examples, many compilers also 
support the `__FUNCTION__` macro, which may not be standard.
__func__ is defined by C99.

#### ERRNO-like method

Functions return error codes to the caller, typically using
the convention that 0 means okay.  If an error is detected,
I've seen two methods used:

* negative error number.  

This can be used when the return value from a function is a signed
integer, and the normal (non-error) values are always positive.  This
makes it easy for the caller to detect an error, such as:

```c
iret=getNumBytesRead();
if(iret<0) {
  // error handling code
} else {
  // normal processing
}
```

* out-of-band error number 

An error number is passed outside the calling mechanism 
via a global errno variable, such as is used by the C-standard
in `errno.h`.  Because this is separate from the normal function
returned parameter, usually these numbers are positive.

#### Error ID Strings

Nothing is as frustrating to a user, or the developer months after the
fact, than an error message like `error: -409`.  Any error module should
really have the ability to include strings, if only in debugging.

An error number can be used to access a string defining the
type of error.  If the errnos are sequential beginning at zero,
this is simply an index into an array of string pointers.  If
we allow error numbers to be negative, or offset from zero, then
some processing has to be done to translate errno into string array
index.  The C-library provides `perror()` and `strerror()` functions
to access these strings.

## Conclusion

This error reporting method will take an errno-like approach, but also
use the `__FILE__`, `__LINE__` and `__FUNCTION__` macros.  If desired, these
can be compiled out for production code to save space.

Furthermore, define the concept of a local (within a single function)
error number. This number can be used as a return integer, which can be
used primarily as a true-false indicator by the calling function.

