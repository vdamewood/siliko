XVCalc Overview

XVCalc is a library for handling mathematical expressions.

XVCalc may be copied, modified, and distributed under the terms of the GNU
GPL, version 3. See the file License.txt for details.

Currently XVCalc supports the following features:

* Integer four-function calculations, such as "2 + 2", "3 * 4", "9 - 3" and
  "8 / 2".
* Negative numbers, such as "-4", "-4 + 20".
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".

XVCalc ignores space and tab characters in its input.

The following features are planned for future versions:

* mixed integer (or long) / float (or double) calculations
* constants: the library will recognize e, and pi constants as numbers
  representing their respective values.
* functions, including:
  * build-in support for an abs() function that will work with the C abs(),
  labs(), and fabs() functions as needed.
  * built-in support for the following functions from C's math.h: sin(), cos(),
    tan(), asin(), acos(), atan(), atan2(), sinh(), cosh(), tanh(), exp(),
    log(), log10(), pow(), sqrt(), ceil(), floor(), and ldexp().
  * user-defined functions through a plug-in interface.
* thread safety, if feasible.
* integer values may be handled as long ints instead.

Once mixed integer/float calculations are implemented, XVCalc will handle
the type of the resulting value in a 'smart' way. Calculations involving
integers that would result in a fractional answer (such as 5/2) and
calculations involving floating-point values will result in floating-point
results. Calculations involving only integers with integer results will give
integer results.

Support for the plug-in interface is currently planned for systems that support
dlopen(3). Support for Windows may be included as well.

XVCalc uses a Flex-generated tokenizer and a Bison-generated parser. Currently,
the parser is not 'pure', and thus XVCalc is not thread safe.

XVcalc Interface

XVCalc's interface consists of the following functions:

char   xvcalc(char * in_string)
int    xvcalc_get_int(void)
float  xvcalc_get_float(void)
char * xvcalc_error_message(void)
void   xvcalc_clean(void)

The xvcalc() functon takes the expression to be evaluated as a typical pointer
to a null-terminated series of characters. When called, the xvcalc() function
parses the string passed to it and sets internal conditions allowing other
functions to be called to retrieve the results and any messages.

The xvcalc() function returns a status code as a char. The following are
currently possible values:

'e':  syntax/parse error      -- An error message will be returned by
                                 xvcalc_error_message(). The pointer returned
                                 points to section of memory allocated with
                                 malloc(). This section of memory will be
                                 free()ed by a call to xvcalc_clean().
'm':  memory alocation error  -- If this is returned, xvcalc was not able to
                                 allocate enough memory to complete an operation
                                 or alocate memory for a syntax error message.
'i':  success, int result     -- The result of the calculation will be returned
                                 by xvcalc_get_int()
'f':  success, float result   -- the result of the calculation will be returned
                                 by xvcalc_get_float(). While this status code
                                 is supported, floating-point operations have
                                 not yet been implemented.

Other values may be added in the future. The 'f' option may change if double
values are used instead. If that happens the xvcalc_get_float() function will
change to the following:

double xvcalc_get_double(void)

Once you have retrieved the value of the calculation, you must clean the state
of the library by calling the xvcalc_clean() function. This function must be
called at least after each call to xvcalc(), before program termination and
before any subsequent calls to xvcalc().

For historical reasons, a Python script is included with the distribution. The
Python script can be used as a binding module for the library and as a script
to test the library. It defines a single function, vldc() which, when called,
returns the value of xvcalc_get_int() or xvcalc_get_float(), which ever is
appropriate. If an error is encountered, it throws an exception.
