/*
 * Functions.h: Built-in functions
 * Copyright 2012, 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined XAVI_FUNCTIONS_H
#define XAVI_FUNCTIONS_H

#include "Value.h"

#if defined __cplusplus
extern "C" {
#endif

XaviValue XaviFunction_add(int argc, XaviValue *argv);
XaviValue XaviFunction_subtract(int argc, XaviValue *argv);
XaviValue XaviFunction_multiply(int argc, XaviValue *argv);
XaviValue XaviFunction_divide(int argc, XaviValue *argv);
XaviValue XaviFunction_power(int argc, XaviValue *argv);
XaviValue XaviFunction_dice(int argc, XaviValue *argv);
XaviValue XaviFunction_abs(int argc, XaviValue *argv);
XaviValue XaviFunction_acos(int argc, XaviValue *argv);
XaviValue XaviFunction_asin(int argc, XaviValue *argv);
XaviValue XaviFunction_atan(int argc, XaviValue *argv);
XaviValue XaviFunction_ceil(int argc, XaviValue *argv);
XaviValue XaviFunction_cos(int argc, XaviValue *argv);
XaviValue XaviFunction_cosh(int argc, XaviValue *argv);
XaviValue XaviFunction_exp(int argc, XaviValue *argv);
XaviValue XaviFunction_floor(int argc, XaviValue *argv);
XaviValue XaviFunction_log(int argc, XaviValue *argv);
XaviValue XaviFunction_log10(int argc, XaviValue *argv);
XaviValue XaviFunction_sin(int argc, XaviValue *argv);
XaviValue XaviFunction_sinh(int argc, XaviValue *argv);
XaviValue XaviFunction_sqrt(int argc, XaviValue *argv);
XaviValue XaviFunction_tan(int argc, XaviValue *argv);
XaviValue XaviFunction_tanh(int argc, XaviValue *argv);

#if defined __cplusplus
}
#endif

#endif /* XAVI_FUNCTIONS_H */
