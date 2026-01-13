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

#include <SilikoCore/Operator.h>
#include <SilikoCore/Math.h>
#include <SilikoCore/Value.h>

// This is the largest value that can be represented by both a
// double and a long long int, assuming the long long int is a
// 64-bit, signed integer, and the double is an IEEE 754-compliant
// binary double-precision floating-point number.
static const double CommonMax = 9223372036854774784.0;

SilikoValue *SilikoOperatorAdd(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssign(result, argv[i]);
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

SilikoValue *SilikoOperatorSubtract(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssign(result, argv[i]);
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

SilikoValue *SilikoOperatorMultiply(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssign(result, argv[i]);
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

SilikoValue *SilikoOperatorDivide(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	SilikoValue *result = SilikoValueCopy(argv[0]);
	if(SilikoValueGetStatus(result) == SilikoValueError)
		return result;
	for(int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
		{
			SilikoValueAssign(result, argv[i]);
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

SilikoValue *SilikoOperatorPower(int argc, SilikoValue **argv)
{
	if (argc < 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	double result = SilikoValueGetReal(argv[0]);
	for (int i = 1; i < argc; i++)
	{
		if(SilikoValueGetStatus(argv[i]) == SilikoValueError)
			return SilikoValueCopy(argv[i]);
		result = pow(result, SilikoValueGetReal(argv[i]));
	}
	return SilikoValueCreateFromReal(result);
}

SilikoValue *SilikoOperatorDice(int argc, SilikoValue **argv)
{
	if (argc != 2)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	if (SilikoValueGetStatus(argv[1]) == SilikoValueError)
		return SilikoValueCopy(argv[1]);

	long long int count = SilikoValueGetInteger(argv[0]);
	long long int faces = SilikoValueGetInteger(argv[1]);

	if (faces == 0)
		return SilikoValueCreateFromInteger(0);

	static int has_seeded = 0;
	if (!has_seeded)
	{
		has_seeded = 1;
		srand((unsigned int)time(NULL));
	}

	long long int result = 0;
	for (int i = 1; i <= count; i++)
		result += (rand() % faces) + 1;
	return SilikoValueCreateFromInteger(result);
}

SilikoValue *SilikoMathAbs(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	switch(SilikoValueGetStatus(argv[0]))
	{
	case SilikoValueInteger:
		return SilikoValueCreateFromInteger(
			llabs(SilikoValueGetInteger(argv[0])));
	case SilikoValueReal:
		return SilikoValueCreateFromReal(
			fabs(SilikoValueGetReal(argv[0])));
	default:
		return SilikoValueCopy(argv[0]);
	}
}

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

SilikoValue *SilikoMathCos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(cos(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathCosh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(cosh(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathExp(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(exp(SilikoValueGetReal(argv[0])));
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

SilikoValue *SilikoMathSin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(sin(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSinh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(sinh(SilikoValueGetReal(argv[0])));
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

SilikoValue *SilikoMathTan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(tan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathTanh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueCreateFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueCopy(argv[0]);

	return SilikoValueCreateFromReal(tanh(SilikoValueGetReal(argv[0])));
}

int SilikoInstallOperators(SilikoEngine *engine)
{
	return SilikoEngineInstallFunction(engine, "add",
			SilikoPureFunctionCreate(SilikoOperatorAdd))
		&& SilikoEngineInstallFunction(engine, "subtract",
			SilikoPureFunctionCreate(SilikoOperatorSubtract))
		&& SilikoEngineInstallFunction(engine, "multiply",
			SilikoPureFunctionCreate(SilikoOperatorMultiply))
		&& SilikoEngineInstallFunction(engine, "divide",
			SilikoPureFunctionCreate(SilikoOperatorDivide))
		&& SilikoEngineInstallFunction(engine, "power",
			SilikoPureFunctionCreate(SilikoOperatorPower))
		&& SilikoEngineInstallFunction(engine, "dice",
			SilikoPureFunctionCreate(SilikoOperatorDice));
}

int SilikoInstallMathFunctions(SilikoEngine *engine)
{
	return SilikoEngineInstallFunction(engine, "abs",
			SilikoPureFunctionCreate(SilikoMathAbs))
		&& SilikoEngineInstallFunction(engine, "acos",
			SilikoPureFunctionCreate(SilikoMathAcos))
		&& SilikoEngineInstallFunction(engine, "asin",
			SilikoPureFunctionCreate(SilikoMathAsin))
		&& SilikoEngineInstallFunction(engine, "atan",
			SilikoPureFunctionCreate(SilikoMathAtan))
		&& SilikoEngineInstallFunction(engine, "ceil",
			SilikoPureFunctionCreate(SilikoMathCeil))
		&& SilikoEngineInstallFunction(engine, "cos",
			SilikoPureFunctionCreate(SilikoMathCos))
		&& SilikoEngineInstallFunction(engine, "cosh",
			SilikoPureFunctionCreate(SilikoMathCosh))
		&& SilikoEngineInstallFunction(engine, "exp",
			SilikoPureFunctionCreate(SilikoMathExp))
		&& SilikoEngineInstallFunction(engine, "floor",
			SilikoPureFunctionCreate(SilikoMathFloor))
		&& SilikoEngineInstallFunction(engine, "log",
			SilikoPureFunctionCreate(SilikoMathLog))
		&& SilikoEngineInstallFunction(engine, "log10",
			SilikoPureFunctionCreate(SilikoMathLog10))
		&& SilikoEngineInstallFunction(engine, "sin",
			SilikoPureFunctionCreate(SilikoMathSin))
		&& SilikoEngineInstallFunction(engine, "sinh",
			SilikoPureFunctionCreate(SilikoMathSinh))
		&& SilikoEngineInstallFunction(engine, "sqrt",
			SilikoPureFunctionCreate(SilikoMathSqrt))
		&& SilikoEngineInstallFunction(engine, "tan",
			SilikoPureFunctionCreate(SilikoMathTan))
		&& SilikoEngineInstallFunction(engine, "tanh",
			SilikoPureFunctionCreate(SilikoMathTanh));
}
