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

SilikoValue *SilikoOperatorAdd(int argc, SilikoValue **argv)
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

SilikoValue *SilikoOperatorSubtract(int argc, SilikoValue **argv)
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

SilikoValue *SilikoOperatorMultiply(int argc, SilikoValue **argv)
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

SilikoValue *SilikoOperatorDivide(int argc, SilikoValue **argv)
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

SilikoValue *SilikoOperatorPower(int argc, SilikoValue **argv)
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

SilikoValue *SilikoOperatorDice(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathAbs(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathAcos(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathAsin(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathAtan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(atan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathCeil(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathCos(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(cos(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathCosh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(cosh(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathExp(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(exp(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathFloor(int argc, SilikoValue **argv)
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

SilikoValue *SilikoMathLog(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(log(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathLog10(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(log10(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSin(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(sin(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSinh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(sinh(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathSqrt(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	if (signbit(SilikoValueGetReal(argv[0])))
		return SilikoValueNewFromError(SilikoErrorDomain);

	return SilikoValueNewFromReal(sqrt(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathTan(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(tan(SilikoValueGetReal(argv[0])));
}

SilikoValue *SilikoMathTanh(int argc, SilikoValue **argv)
{
	if (argc != 1)
		return SilikoValueNewFromError(SilikoErrorFunctionArguments);

	if (SilikoValueGetStatus(argv[0]) == SilikoValueError)
		return SilikoValueNewCopy(argv[0]);

	return SilikoValueNewFromReal(tanh(SilikoValueGetReal(argv[0])));
}

int SilikoInstallOperators(SilikoEngine *engine)
{
	return SilikoEngineInstallFunction(engine, "add",
			SilikoPureFunctionNew(SilikoOperatorAdd))
		&& SilikoEngineInstallFunction(engine, "subtract",
			SilikoPureFunctionNew(SilikoOperatorSubtract))
		&& SilikoEngineInstallFunction(engine, "multiply",
			SilikoPureFunctionNew(SilikoOperatorMultiply))
		&& SilikoEngineInstallFunction(engine, "divide",
			SilikoPureFunctionNew(SilikoOperatorDivide))
		&& SilikoEngineInstallFunction(engine, "power",
			SilikoPureFunctionNew(SilikoOperatorPower))
		&& SilikoEngineInstallFunction(engine, "dice",
			SilikoPureFunctionNew(SilikoOperatorDice));
}

int SilikoInstallMathFunctions(SilikoEngine *engine)
{
	return SilikoEngineInstallFunction(engine, "abs",
			SilikoPureFunctionNew(SilikoMathAbs))
		&& SilikoEngineInstallFunction(engine, "acos",
			SilikoPureFunctionNew(SilikoMathAcos))
		&& SilikoEngineInstallFunction(engine, "asin",
			SilikoPureFunctionNew(SilikoMathAsin))
		&& SilikoEngineInstallFunction(engine, "atan",
			SilikoPureFunctionNew(SilikoMathAtan))
		&& SilikoEngineInstallFunction(engine, "ceil",
			SilikoPureFunctionNew(SilikoMathCeil))
		&& SilikoEngineInstallFunction(engine, "cos",
			SilikoPureFunctionNew(SilikoMathCos))
		&& SilikoEngineInstallFunction(engine, "cosh",
			SilikoPureFunctionNew(SilikoMathCosh))
		&& SilikoEngineInstallFunction(engine, "exp",
			SilikoPureFunctionNew(SilikoMathExp))
		&& SilikoEngineInstallFunction(engine, "floor",
			SilikoPureFunctionNew(SilikoMathFloor))
		&& SilikoEngineInstallFunction(engine, "log",
			SilikoPureFunctionNew(SilikoMathLog))
		&& SilikoEngineInstallFunction(engine, "log10",
			SilikoPureFunctionNew(SilikoMathLog10))
		&& SilikoEngineInstallFunction(engine, "sin",
			SilikoPureFunctionNew(SilikoMathSin))
		&& SilikoEngineInstallFunction(engine, "sinh",
			SilikoPureFunctionNew(SilikoMathSinh))
		&& SilikoEngineInstallFunction(engine, "sqrt",
			SilikoPureFunctionNew(SilikoMathSqrt))
		&& SilikoEngineInstallFunction(engine, "tan",
			SilikoPureFunctionNew(SilikoMathTan))
		&& SilikoEngineInstallFunction(engine, "tanh",
			SilikoPureFunctionNew(SilikoMathTanh));
}
