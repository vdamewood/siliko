XVCalc Overview

XVCalc is a library for handling mathematical expressions.

XVCalc may be copied, modified, and distributed under the terms of the GNU
LGPL, version 3 or later. See the files LGPL-3.0.txt and GPL-3.0.txt for
details. The source distribution of XVCalc also includes test programs.
These programs may be copied, modified, and distributed under the terms of
the GNU GPL, version 3 or later. See the file GPL-3.0.txt for details.

Currently XVCalc supports the following features:

* Mixed integer and floating-point four-function calculations, such as "2 + 2",
  "3 * 4.0", "9.0 - 3.0" and "8 / 2".
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".
* Functions, currently XVcalc supports  abs() and sqrt() functions.
* XVCalc ignores space and tab characters in its input.

The following features are planned for the first release:

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
the parser is not 'pure', and some other components have static 'state' values.
Thus XVCalc is not thread safe.

XVcalc Interface

XVCalc's interface consists of the following functions:

void               XvcOpen();
void               XvcClose();
enum XvcStatus     XvcParse(const char * StringToBeParsed)
int                XvcGetInteger(void)
float              XvcGetFloat(void)

Call XvcOpen() before calling any other functions. Call XvcClose()
after you are finished with XVCalc and before your program terminates. These
functions are used to setup and tear down resources used by XVCalc.

Other symbols starting with Xvc will be found in the library but
are not part of the public interface, so don't use them.

The XVCParse() function takes a pointer to the first of a series of characters
that represent a mathematical expression, terminated by a null character. When
called, the XvxParse() function parses the string passed to it and sets
internal conditions allowing other functions to be called to retrieve the
results and any messages.

The XvcParse() function returns a value in the XvcStatus enumeration. The
XvcStatus enumeration is found in XVCalc.h with comments on what each
status means. Depending on the status returned by XvcParse(), you may retrieve
the value using either XvcGetInteger() or XvcGetFloat().

Known Issue / Goals

* The API is more complicated than it needs to be.
* The CMakeFile.txt file in libxvcalc doesn't generate files with Flex/Bison
  correctly.
* Flex and Bison may not be the best solution if XVCalc is going to support
  Windows.
* Not all functions have been implemented.
* The method for selecting a function needs to be replaced by a hash table.
