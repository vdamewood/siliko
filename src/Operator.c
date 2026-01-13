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
#include <stdlib.h>
#include <time.h>

#include <SilikoCore/Operator.h>
#include <SilikoCore/Value.h>

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
