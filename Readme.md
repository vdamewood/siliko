# Siliko

<https://vdamewood.com/software/projects/Siliko>

Siliko is library written in C for parsing and evaluating
mathematical expressions. It was made to be an example for teaching
software development.

## Interface Programs

This project includes interface programs called Silikilo, and
Silikujo. Silikilo is a command-line application, and Silikujo is
a GUI program. There are three different versions of Silikujo:

* For Windows, using the Win32 API
* for macOS, using Cocoa
* for Linux and FreeBSD using GTKmm

Other Unix-like systems, may work using the version for Linux and
FreeBSD, but they have not been tested.

## The Language

Siliko has a built-in infix parser that handles the following:

* Four-function calculations, such as `2 + 2`, `3 * 4.0`, `9.0 - 3.0`
  and `8 / 2`.
* Exponents with a `^` operator. For example `2^3` is `8`.
* Negative numbers, such as `-4`, `-4.0 + 20`.
* constants, `e` and `pi` may be used as numbers.
* parenthetic expressions, such as `2 * (3 + 4)`.
* Random number generation using a `d` operator to simulate die rolls.
  such as `3d6` or `1d20`.
* Functions: `abs()`, `acos()`, `asin()`, `atan()`, `ceil()`, `cos()`,
  `cosh()`, `exp()`, `floor()`, `log()`, `log10()`, `sin()`, `sinh()`,
  `sqrt()`, `tan()`, and `tanh()`.

Siliko ignores space and tab characters in its input, except to
separate tokens.

Siliko supports integer and floating-point arithmetic. In most
situations, integer expressions will yield integer results, and
floating-point expressions will yield floating-point results. The
exceptions are that the above-mentioned functions always return
floating-point values, and any integer division where the result would
be truncated will result in a floating-point quotient.

## Building Siliko

Siliko includes files to use CMake <http://www.cmake.org> to generate
a build system. The following build systems are regularly tested with
Siliko:

* Ninja
* Unix Makefiles
* NMake Makefiles (on Windows only)

Other build systems, including IDE-based projects, should work, but
don't receive extensive testing. Additionally, the build process is
tested with the following compilers.

* Clang (including both the standard clang distribution and Apple Clang
    on macOS)
* gcc
* Microsoft Visual C++ (on Windows)

The build system is tested with the following compilers:

* Linux: GCC and Clang
* FreeBSD: Clang
* OS X: Clang
* Windows: Visual C++ (cl.exe)

The command-line interface program, Silikilo, can use GNU Readline
to improve the user experience. The GUI program for Linux and FreeBSD
uses GTKmm. Make sure you have these libraries installed, including
their 'development' components. In Windows, you will need to setup
appropriate environment variables so cmake can find your compile,
headers, and libraries.

Under normal conditions, you can build Siliko by first making a
an empty directory in which to build it, then running cmake with the
following command:

```Command-Line
cmake <Path to Siliko Source>
```

On Mac OS X, FreeBSD, and Linux, the default build-system generator is
the "Unix Makefiles" generator.  On Windows, the default is to setup a
Visual Studio solution. Consult the CMake documentation to select a
different build system generator if you do not wish to use the default.

### Building an Installation Package

Siliko is configured to build an installer using the WiX Toolset
<https://wixtoolset.org> on Windows, and a Drag-and-Drop disk image on
macOS. For other systems, it's assumed that an immediate installation
suffices, and that package maintainers for each system will build
installation packages using their own processes. To build an
installation package on Windows and macOS, build the target "package".

## License Information

Copyright 2019 Vincent Damewood

Siliko is licensed under the terms of the GNU Lesser General Public
license, version 3 or later. Silikilo and Silikujo are licensed
under the terms of the GNU General Public license, version 3 or later.

Siliko, Silikilo, and Silikujo are distributed in the hope that
they will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
