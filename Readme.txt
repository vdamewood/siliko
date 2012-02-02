Xavi Overview

Xavi is a library for handling mathematical expressions.

Xavi may be copied, modified, and distributed under the terms of the GNU
LGPL, version 3 or later. See the files LGPL-3.0.txt and GPL-3.0.txt for
details. The source distribution of Xavi also includes test programs.
These programs may be copied, modified, and distributed under the terms of
the GNU GPL, version 3 or later. See the file GPL-3.0.txt for details.

Currently Xavi supports the following features:

* Mixed integer and floating-point four-function calculations, such as "2 + 2",
  "3 * 4.0", "9.0 - 3.0" and "8 / 2".
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".
* Functions, currently Xavi supports  abs() and sqrt() functions.
* Xavi ignores space and tab characters in its input.

The following features are planned for the first release:

* built-in support for the following functions from C's math.h: sin(), cos(),
  tan(), asin(), acos(), atan(), atan2(), sinh(), cosh(), tanh(), exp(),
  log(), log10(), pow(), sqrt(), ceil(), floor(), and ldexp().
* user-defined functions through a plug-in interface.
* thread safety, if feasible.

Currently, Xavi handles mixing integers and floating-point numbers in a
manner similar to other languages: operations involving only integers will give
and integer result while operations involving a floating-point number will
give a floating-point result. Eventually, Xavi will handle the type of the
resulting value in a 'smart' way. Calculations involving integers that would
result in a fractional answer (such as 5/2) and calculations involving
floating-point values will result in floating-point results. Calculations
involving only integers with integer results will give integer results.

Support for the plug-in interface is currently planned for systems that support
dlopen(3). Support for Windows may be included as well.

Xavi uses a Flex-generated tokenizer and a Bison-generated parser. Currently,
the parser is not 'pure', and some other components have static 'state' values.
Thus Xavi is not thread safe.

Xavi Interface

Xavi's interface consists of the following functions:

void               XaviOpen();
void               XaviClose();
struct XaviNumber  XaviParse(const char * StringToBeParsed)

Call XaviOpen() before calling any other functions. Call XaviClose()
after you are finished with Xavi and before your program terminates. These
functions are used to setup and tear down resources used by Xavi.

Other symbols starting with Xavi will be found in the library but
are not part of the public interface, so don't use them.

The XaviParse() function takes a pointer to the first of a series of characters
that represent a mathematical expression, terminated by a null character. When
called, the XvxParse() function parses the string passed to it and returns
an XaviNumber structure. The Structure has two members, a status code as an
enum XaviStatus (status), and the value to which the expression evaluates as
an anonymous union of int (i) and float (f). XaviStatus enumeration is found
in Xavi.h with comments on what each status means.

Known Issue / Goals

* Documentation needs to be moved into Docbook XML files.
* Not all functions have been implemented.
* The GUI program needs to be documented.
