Xavi Overview

Xavi is a library for handling mathematical expressions. Xavi's home on the
web is <http://damewood.us/projects/xavi>.

Xavi may be copied, modified, and distributed under the terms of the GNU
LGPL, version 3 or later. See the files LGPL-3.0.txt and GPL-3.0.txt for
details. The source distribution of Xavi also includes test programs.
These programs may be copied, modified, and distributed under the terms of
the GNU GPL, version 3 or later. See the file GPL-3.0.txt for details.
Xavista uses Qt <http://> under the terms of the GNU GPL 3.0.

Currently Xavi supports the following features:

* Mixed integer and floating-point four-function calculations, such as "2 + 2",
  "3 * 4.0", "9.0 - 3.0" and "8 / 2".
* Exponents with a ^ operator. For example "2^3" is 8.
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".
* Functions: Xavi supports the following functions from the C standard library:
  abs(), acos(), asin(), atan(), ceil(), cos(), cosh(), exp(), floor(),
  log(),log10(), sin(), sinh(), sqrt(), tan(), and tanh().

  Xavi's abs() function will use C's abs() or fabs() as appropriate.
* Xavi ignores space and tab characters in its input.

When mixing number types, Xavi will give integer results in calculations
involving only integers and floating-point results in calculations involving
either integers and floating-point numbers, or only floating-point numbers.
If division involves two intergers where the result would not be a whole
number, a floating-point value is returned.


Xavi Interface

Xavi's interface consists of the following functions:

void               XaviOpen();
void               XaviClose();
struct XaviNumber  XaviParse(const char * StringToBeParsed)

Call XaviOpen() before calling XaviParse(). Call XaviClose()
after you are finished with Xavi and before your program terminates. These
functions are used to setup and tear down resources used by Xavi.

Other symbols starting with "Xavi" will be found in the library but
are not part of the public interface, so don't use them.

The XaviParse() function takes a pointer to the first of a series of characters
that represent a mathematical expression, terminated by a null character. When
called, the XvxParse() function parses the string passed to it and returns
a XaviNumber structure. The Structure has two members, a status code as an
enum XaviStatus (status), and the value to which the expression evaluates as
an anonymous union of int (i) and float (f). XaviStatus enumeration is found
in Xavi.h with comments on what each status means.
