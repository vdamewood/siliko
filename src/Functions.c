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


#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Functions.h"

SilikoValue *SilikoFunction_add(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueGetInteger(result)
				+ SilikoValueGetInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueGetReal(result)
				+ SilikoValueGetReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_subtract(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueGetInteger(result)
				- SilikoValueGetInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueGetReal(result)
				- SilikoValueGetReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_multiply(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueGetInteger(result)
				* SilikoValueGetInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueGetReal(result)
				* SilikoValueGetReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_divide(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueCopy(result, argv[i]);
			return result;
		}

		if(SilikoValueGetReal(argv[1]) == 0.0)
		{
			SilikoValueAssignError(result, SilikoErrorZeroDivision);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger
			&& SilikoValueGetInteger(result)
				% SilikoValueGetInteger(argv[i])
				== 0)
		{
			SilikoValueAssignInteger(result,
				SilikoValueGetInteger(result)
				/ SilikoValueGetInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueGetReal(result)
				/ SilikoValueGetReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_power(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	double result = SilikoValueGetReal(argv[0]);
	for (int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
			return SilikoValueNewCopy(argv[i]);
		result = pow(result, SilikoValueGetReal(argv[i]));
	}
	return SilikoValueNewFromReal(result);
}

SilikoValue *SilikoFunction_dice(int argc, SilikoValue **argv)
{
	if (argc != 2)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	if (SilikoValueGetStatus(argv[1]) == SilikoValueError)
		return SilikoValueNewCopy(argv[1]);

	long long int count = SilikoValueGetInteger(argv[0]);
	long long int faces = SilikoValueGetInteger(argv[1]);

	if (faces == 0)
		return SilikoValueNewFromInteger(0);

	static int has_seeded = 0;
	if (!has_seeded)
	{
		has_seeded = 1;
		srand((unsigned int)time(NULL));
	}

	long long int result = 0;
	for (int i = 1; i <= count; i++)
		result += (rand() % faces) + 1;
	return SilikoValueNewFromInteger(result);
}

SilikoValue *SilikoFunction_abs(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueInteger:
		return SilikoValueNewFromInteger(
			llabs(SilikoValueGetInteger(argv[0])));
	case SilikoValueReal:
		return SilikoValueNewFromReal(
			fabs(SilikoValueGetReal(argv[0])));
	default:
		return SilikoValueNewCopy(argv[0]);
	}
}

SilikoValue *SilikoFunction_acos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	double input = SilikoValueGetReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueNewFromError(SilikoErrorDomain);

	return SilikoValueNewFromReal(acos(input));
}

SilikoValue *SilikoFunction_asin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	float input = SilikoValueGetReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueNewFromError(SilikoErrorDomain);

	return SilikoValueNewFromReal(asin(input));
}

SilikoValue *SilikoFunction_atan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(atan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_ceil(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueNewCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = ceil(SilikoValueGetReal(argv[0]));
		if (result <= LLONG_MAX && result >= LLONG_MIN)
			return SilikoValueNewFromInteger((long long int) result);
		else
			return SilikoValueNewFromReal(result);
	}
	}
}

SilikoValue *SilikoFunction_cos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(cos(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_cosh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(cosh(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_exp(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(exp(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_floor(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueNewCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = floor(SilikoValueGetReal(argv[0]));
		if (result <= LLONG_MAX && result >= LLONG_MIN)
			return SilikoValueNewFromInteger((long long int) result);
		else
			return SilikoValueNewFromReal(result);
	}
	}
}

SilikoValue *SilikoFunction_log(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(log(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_log10(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(log10(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_sin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(sin(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_sinh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(sinh(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_sqrt(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	if (signbit(SilikoValueGetReal(argv[0])))
		return SilikoValueNewFromError(SilikoErrorDomain);

	return SilikoValueNewFromReal(sqrt(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_tan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(tan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoFunction_tanh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(tanh(SilikoValueGetReal(argv[0])));
}
