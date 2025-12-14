/* Functions.c: Built-in functions
 * Copyright 2012-2025 Vincent Damewood
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

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Functions.h"

SilikoValue *SilikoFunction_add(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssignCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueToInteger(result)
				+ SilikoValueToInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueToReal(result)
				+ SilikoValueToReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_subtract(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssignCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueToInteger(result)
				- SilikoValueToInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueToReal(result)
				- SilikoValueToReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_multiply(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssignCopy(result, argv[i]);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger)
		{
			SilikoValueAssignInteger(result,
				SilikoValueToInteger(result)
				* SilikoValueToInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueToReal(result)
				* SilikoValueToReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_divide(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueNewCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssignCopy(result, argv[i]);
			return result;
		}

		if(SilikoValueToReal(argv[1]) == 0.0)
		{
			SilikoValueAssignError(result, SilikoErrorZeroDivision);
			return result;
		}

		if (SilikoValueGetStatus(result) == SilikoValueInteger
			&& SilikoValueGetStatus(argv[i]) == SilikoValueInteger
			&& SilikoValueToInteger(result)
				% SilikoValueToInteger(argv[i])
				== 0)
		{
			SilikoValueAssignInteger(result,
				SilikoValueToInteger(result)
				/ SilikoValueToInteger(argv[i]));
		}
		else
		{
			SilikoValueAssignReal(result,
				SilikoValueToReal(result)
				/ SilikoValueToReal(argv[i]));
		}
	}
	return result;
}

SilikoValue *SilikoFunction_power(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	double result = SilikoValueToReal(argv[0]);
	for (int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
			return SilikoValueNewCopy(argv[i]);
		result = pow(result, SilikoValueToReal(argv[i]));
	}
	return SilikoValueNewReal(result);
}

SilikoValue *SilikoFunction_dice(int argc, SilikoValue **argv)
{
	if (argc != 2)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	if (SilikoValueGetStatus(argv[1]) == SilikoValueError)
		return SilikoValueNewCopy(argv[1]);

	long long int count = SilikoValueToInteger(argv[0]);
	long long int faces = SilikoValueToInteger(argv[1]);

	if (faces == 0)
		return SilikoValueNewInteger(0);

	static int has_seeded = 0;
	if (!has_seeded)
	{
		has_seeded = 1;
		srand((unsigned int)time(NULL));
	}

	long long int result = 0;
	for (int i = 1; i <= count; i++)
		result += (rand() % faces) + 1;
	return SilikoValueNewInteger(result);
}

SilikoValue *SilikoFunction_abs(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueInteger:
		return SilikoValueNewInteger(
			llabs(SilikoValueToInteger(argv[0])));
	case SilikoValueReal:
		return SilikoValueNewReal(fabs(SilikoValueToReal(argv[0])));
	default:
		return SilikoValueNewCopy(argv[0]);
	}
}

SilikoValue *SilikoFunction_acos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	double input = SilikoValueToReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueNewError(SilikoErrorDomain);

	return SilikoValueNewReal(acos(input));
}

SilikoValue *SilikoFunction_asin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	float input = SilikoValueToReal(argv[0]);
	if (input < -1.0 || input > 1.0)
		return SilikoValueNewError(SilikoErrorDomain);

	return SilikoValueNewReal(asin(input));
}

SilikoValue *SilikoFunction_atan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(atan(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_ceil(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueNewCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = ceil(SilikoValueToReal(argv[0]));
		if (result <= LLONG_MAX && result >= LLONG_MIN)
			return SilikoValueNewInteger((long long int) result);
		else
			return SilikoValueNewReal(result);
	}
	}
}

SilikoValue *SilikoFunction_cos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(cos(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_cosh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(cosh(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_exp(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(exp(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_floor(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueError:
	case SilikoValueInteger:
		return SilikoValueNewCopy(argv[0]);
	case SilikoValueReal:
	{
		double result = floor(SilikoValueToReal(argv[0]));
		if (result <= LLONG_MAX && result >= LLONG_MIN)
			return SilikoValueNewInteger((long long int) result);
		else
			return SilikoValueNewReal(result);
	}
	}
}

SilikoValue *SilikoFunction_log(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(log(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_log10(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(log10(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_sin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(sin(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_sinh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(sinh(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_sqrt(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	if (signbit(SilikoValueToReal(argv[0])))
		return SilikoValueNewError(SilikoErrorDomain);

	return SilikoValueNewReal(sqrt(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_tan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(tan(SilikoValueToReal(argv[0])));
}

SilikoValue *SilikoFunction_tanh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewReal(tanh(SilikoValueToReal(argv[0])));
}
