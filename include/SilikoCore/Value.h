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

SILIKO_EXPORT_C_API

SILIKOCORE_EXPORT
SilikoValue *SilikoValueCreate(void);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueCreateFromError(enum SilikoError source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueCreateFromInteger(long long int source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueCreateFromReal(double source);
SILIKOCORE_EXPORT
SilikoValue *SilikoValueCopy(const SilikoValue *source);
SILIKOCORE_EXPORT
void SilikoValueDestroy(SilikoValue *object);

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
void SilikoValueAssign(
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

SILIKO_END_C_API

#endif // SILIKO_CORE_VALUE_H
