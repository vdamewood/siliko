XVCalc Overview

XVCalc is a library for handling mathematical expressions.

XVCalc may be copied, modified, and distributed under the terms of the GNU
GPL, version 3. See the file License.txt for details.

Currently XVCalc supports the following features:

* Mixed integer and floating-point four-function calculations, such as "2 + 2",
  "3 * 4.0", "9.0 - 3.0" and "8 / 2".
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".
* Functions, currently XVcalc supports a basic abs() function. Function handling
  is slightly buggy right now, however.
XVCalc ignores space and tab characters in its input.

The following features are planned for future versions:

* built-in support for the following functions from C's math.h: sin(), cos(),
  tan(), asin(), acos(), atan(), atan2(), sinh(), cosh(), tanh(), exp(),
  log(), log10(), pow(), sqrt(), ceil(), floor(), and ldexp().
* user-defined functions through a plug-in interface.
* thread safety, if feasible.

Currently, XVCalc handles mixing integers and floating-point numbers in a
manner similar to other languages: operations involving only integers will give
and integer result while operations involving a floating-point number will
give a floating-point result. Eventually, XVCalc will handle the type of the
resulting value in a 'smart' way. Calculations involving integers that would
result in a fractional answer (such as 5/2) and calculations involving
floating-point values will result in floating-point results. Calculations
involving only integers with integer results will give integer results.

Support for the plug-in interface is currently planned for systems that support
dlopen(3). Support for Windows may be included as well.

XVCalc uses a Flex-generated tokenizer and a Bison-generated parser. Currently,
the parser is not 'pure', and thus XVCalc is not thread safe.

XVcalc Interface

XVCalc's interface consists of the following functions:

enum xvcalc_status xvcalc(char * in_string)
int                xvcalc_get_int(void)
float              xvcalc_get_float(void)
char             * xvcalc_error_message(void)
void               xvcalc_clean(void)

The xvcalc_status ennumeration is found in xvcalc.h with comments on what each
status means.

Other symbols starting with xvcalc_ will be found in the library but are not
part of the public interface, so don't use them.

The xvcalc() functon takes the expression to be evaluated as a typical pointer
to a null-terminated series of characters. When called, the xvcalc() function
parses the string passed to it and sets internal conditions allowing other
functions to be called to retrieve the results and any messages.

The xvcalc() function returns a status code. The meaning of these status
codes are detaild in xvcalc.h.

Once you have retrieved the value of the calculation, you must clean the state
of the library by calling the xvcalc_clean() function. This function must be
called at least after each call to xvcalc(), before program termination and
before any subsequent calls to xvcalc().

Known Issues

VXcalc will leak memory if a syntax error is encountered.
