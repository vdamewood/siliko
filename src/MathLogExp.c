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


SilikoValue *SilikoMathExp(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(exp(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathLog(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(log(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathLog10(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(log10(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSqrt(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	if (signbit(SilikoValueGetReal(argv[0])))
		return SilikoValueCreateFromError(SilikoErrorDomain);

	return SilikoValueCreateFromReal(sqrt(SilikoValueGetReal(argv[0])));
}
