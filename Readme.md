# Siliko Project Overview

<http://damewood.us/projects/Siliko>

Siliko project is an example project for teaching programmers how to manage
and organize their code, and is a library, written in C, for processing and
evaluating mathematical expressions using mixed integer and floating-point
arithmetic. C++ Developers may be intereted in Silikego, a C++ library that is
virtually identical to Siliko.

Interface programs are also included in the project. These programs
demonstrate how to use the base library. There is also a companion book for the
Siliko and Silikego. Information on finding the book is on the Siliko Project
page linked above.

For instructions on how to build Siliko, see the file Build.txt.

The library may be copied, modified, and distributed under the terms of the GNU
Lesser General Public License version 3 or later. See the file LGPL-3.0.txt for
details. The interface programs may be copied, modified, and distributed under
the terms of the GNU General Public License version 3 or later. See the file
GPL-3.0.txt for details.

Siliko has some level of extendability. Using its default components, Siliko
supports infix-notation arithmatic expressions with the following features:

* Four-function calculations, such as `2 + 2`, `3 * 4.0`, `9.0 - 3.0` and
  `8 / 2`.
* Exponents with a `^` operator. For example `2^3` is `8`.
* Negative numbers, such as `-4`, `-4.0 + 20`.
* constants, `e` and `pi` may be used as numbers.
* parenthetic expressions, such as `2 * (3 + 4)`.
* Random number generation using a `d` operator to simulate die rolls. such as
  `3d6` or `1d20`.
* Functions: `abs()`, `acos()`, `asin()`, `atan()`, `ceil()`, `cos()`,
  `cosh()`, `exp()`, `floor()`, `log()`, `log10()`, `sin()`, `sinh()`,
  `sqrt()`, `tan()`, and `tanh()`.
* Siliko ignores space and tab characters in its input, except to
  separate tokens.

When mixing numeric types, Siliko will give integer results in operations
involving only integers and will give floating-point results in calculations
involving floating-point numbers whether integers are also included or not. If
division involves two integers where the result would not be a whole number, the
operation will yeald a floating-point value.
