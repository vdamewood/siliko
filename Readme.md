Xavi Project Overview

Xavi and XaviPP are libraries for processing and evaluating mathematical
expressions using mixed integer and floating-point arithmetic. Xavi is
written in C, and XaviPP is written in C++. They are both part of the overall
Xavi project. the Xavi Project's home on the web is
<http://damewood.us/projects/xavi>. For instructions on how to build them, see
the file Build.txt, included in the source distribution.

Xavi and XaviPP may be copied, modified, and distributed under the terms of the
GNU Lesser General Public License version 3 or later. The Xavi Project source
distribution includes tests and example programs that work with Xavi and XaviPP.
These programs may be copied, modified, and distributed under the terms of the
GNU General Public License version 3 or later. See the file GPL-3.0.txt for
details.

Using their default components, Xavi and XaviPP support infix-notation arithmatic
expressions with the following features:

* Four-function calculations, such as "2 + 2", "3 * 4.0", "9.0 - 3.0" and
  "8 / 2".
* Exponents with a ^ operator. For example "2^3" is 8.
* Negative numbers, such as "-4", "-4.0 + 20".
* constants, e and pi may be used as numbers.
* parenthetic expressions, such as "2 * (3 + 4)".
* Random number generation using a d operator to simulate die rolls. such as
  "3d6" or "1d20".
* Functions: abs(), acos(), asin(), atan(), ceil(), cos(), cosh(), exp(),
  floor(), log(),log10(), sin(), sinh(), sqrt(), tan(), and tanh().
* Xavi and XaviPP ignore space and tab characters in their input, except to
  separate tokens.

When mixing numeric types, Xavi and XaviPP will give integer results in
calculations involving only integers and floating-point results in calculations
involving either integers and floating-point numbers, or only floating-point
numbers. If division involves two integers where the result would not be a whole
number, a floating-point value is returned.
