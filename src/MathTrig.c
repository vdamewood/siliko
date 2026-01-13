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

SilikoValue *SilikoMathAcos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	double input = SilikoValueGetReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueCreateFromError(SilikoErrorDomain);

	return SilikoValueCreateFromReal(acos(input));
}

SilikoValue *SilikoMathAsin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	float input = SilikoValueGetReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueCreateFromError(SilikoErrorDomain);

	return SilikoValueCreateFromReal(asin(input));
}

SilikoValue *SilikoMathAtan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(atan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathCos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(cos(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(sin(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathTan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(tan(SilikoValueGetReal(argv[0])));
}
