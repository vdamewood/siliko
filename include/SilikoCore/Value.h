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


#if !defined SILIKO_CORE_VALUE_H
#define SILIKO_CORE_VALUE_H

#include <SilikoCore/Api.h>

enum SilikoError
{
	SilikoErrorNone,
	SilikoErrorMemory,
	SilikoErrorSyntax,
	SilikoErrorZeroDivision,
	SilikoErrorFunctionName,
	SilikoErrorFunctionArguments,
	SilikoErrorDomain,
	SilikoErrorRange
};

enum SilikoValueStatus
{
	SilikoValueError,
	SilikoValueInteger,
	SilikoValueReal
};

typedef struct SilikoValue SilikoValue;

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT
SilikoValue *SilikoValueNew(void);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueNewFromError(enum SilikoError source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueNewFromInteger(long long int source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueNewFromReal(double source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueNewCopy(const SilikoValue *source);
SILIKOCORE_EXPORT
void SilikoValueDelete(SilikoValue *object);

SILIKOCORE_EXPORT
void SilikoValueAssignError(
	SilikoValue *object,
	enum SilikoError source);
SILIKOCORE_EXPORT
void SilikoValueAssignInteger(
	SilikoValue *object,
	long long int source);
SILIKOCORE_EXPORT
void SilikoValueAssignReal(
	SilikoValue *object,
	double source);
SILIKOCORE_EXPORT
void SilikoValueCopy(
	SilikoValue *object,
	const SilikoValue *source);

SILIKOCORE_EXPORT
enum SilikoValueStatus SilikoValueGetStatus(
	const SilikoValue *object);
SILIKOCORE_EXPORT
enum SilikoError SilikoValueGetError(
	const SilikoValue *object);
SILIKOCORE_EXPORT
long long int SilikoValueGetInteger(
	const SilikoValue *object);
SILIKOCORE_EXPORT
double SilikoValueGetReal(
	const SilikoValue *object);
SILIKOCORE_EXPORT
void SilikoValueNegate(
	SilikoValue *object);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_VALUE_H
