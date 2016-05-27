# Siliko Project Overview

<http://damewood.us/projects/Siliko>

The Siliko project is an example project for teaching programmers how to manage
and organize their code, as well as streamline the development process. As
the main examples, Siliko and SilikoPP are libraries for processing and evaluating
mathematical expressions using mixed integer and floating-point arithmetic.
Siliko is written in C, and SilikoPP is written in C++. They are mutually redundant.
So a C programmer can safely ignore SilikoPP and a C++ programmer can ignore Siliko.
The Siliko Project also includes example programs and tests that interact with the
libraries. These programs offer a minimal user interface, and demonstrate how to
use the base libraries. There is also a companion book to the Siliko project.
Information on finding the book is on the Siliko Project page linked above.

For instructions on how to build the Siliko project, see the file Build.txt.

The libraries, Siliko and SilikoPP, may be copied, modified, and distributed under
the terms of the GNU Lesser General Public License version 3 or later. See the
file LGPL-3.0.txt for details. Tests and example programs may be copied,
modified, and distributed under the terms of the GNU General Public License
version 3 or later. See the file GPL-3.0.txt for details.

Siliko and SilikoPP have some level of extendability. Using their default
components, Siliko and SilikoPP support infix-notation arithmatic expressions with
the following features:

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
* Siliko and SilikoPP ignore space and tab characters in their input, except to
  separate tokens.

When mixing numeric types, Siliko and SilikoPP will give integer results in
calculations involving only integers and floating-point results in calculations
involving either integers and floating-point numbers, or only floating-point
numbers. If division involves two integers where the result would not be a whole
number, they return a floating-point value.
