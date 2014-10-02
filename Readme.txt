Xavi Overview

Xavi is a C++ library for processing and evaluating mathematical expressions
using mixed integer and floating-point arithmetic. Xavi's home on the web is
<http://damewood.us/projects/xavi>.

Xavi may be copied, modified, and distributed under the terms of the GNU
LGPL, version 3 or later. See the files LGPL-3.0.txt and GPL-3.0.txt for
details. The source distribution of Xavi also includes test programs.
These programs may be copied, modified, and distributed under the terms of
the GNU GPL, version 3 or later. See the file GPL-3.0.txt for details.
Xavista uses Qt <http://qt-project.org/> under the terms of the GNU GPL 3.0.

Using its default components Xavi supports infix-notation arithmatic expressions
with the following features:

* Four-function calculations, such as "2 + 2", "3 * 4.0", "9.0 - 3.0" and
  "8 / 2".
* Exponents with a ^ operator. For example "2^3" is 8.
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate dice rolls. such as
  "3d6" or "1d20".
* Functions: Xavi supports the following functions from the C++ standard
  library: abs(), acos(), asin(), atan(), ceil(), cos(), cosh(), exp(),
  floor(), log(),log10(), sin(), sinh(), sqrt(), tan(), and tanh().
* Xavi ignores space and tab characters in its input.

When mixing numeric types, Xavi will give integer results in calculations
involving only integers and floating-point results in calculations involving
either integers and floating-point numbers, or only floating-point numbers.
If division involves two integers where the result would not be a whole
number, a floating-point value is returned.

The programs Xavita and Xavista are included as example programs to interface
with Xavi. Xavita runs on the command line while Xavista is a GUI program using
the Qt framework.
