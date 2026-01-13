// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Siliko.

// Siliko is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Siliko is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Siliko. If not, see
// <http://www.gnu.org/licenses/>.


#include <math.h>

#include <SilikoCore/Math.h>
#include <SilikoCore/Value.h>

// This is the largest value that can be represented by both a
// double and a long long int, assuming the long long int is a
// 64-bit, signed integer, and the double is an IEEE 754-compliant
// binary double-precision floating-point number.
static const double CommonMax = 9223372036854774784.0;

SilikoValue *SilikoMathCeil(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = ceil(SilikoValueGetReal(argv[0]));
		if (result <= CommonMax && result >= -CommonMax)
			return SilikoValueCreateFromInteger((long long int) result);
		else
			return SilikoValueCreateFromReal(result);
	}
	}
}

SilikoValue *SilikoMathFloor(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = floor(SilikoValueGetReal(argv[0]));
		if (result <= CommonMax && result >= -CommonMax)
			return SilikoValueCreateFromInteger((long long int) result);
		else
			return SilikoValueCreateFromReal(result);
	}
	}
}

SilikoValue *SilikoMathRound(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = round(SilikoValueGetReal(argv[0]));
		if (result <= CommonMax && result >= -CommonMax)
			return SilikoValueCreateFromInteger((long long int) result);
		else
			return SilikoValueCreateFromReal(result);
	}
	}
}

SilikoValue *SilikoMathTrunc(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = trunc(SilikoValueGetReal(argv[0]));
		if (result <= CommonMax && result >= -CommonMax)
			return SilikoValueCreateFromInteger((long long int) result);
		else
			return SilikoValueCreateFromReal(result);
	}
	}
}
