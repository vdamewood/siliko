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


#if !defined SILIKO_CORE_TOKEN_H
#define SILIKO_CORE_TOKEN_H

#include <SilikoCore/Api.h>

typedef struct SilikoToken SilikoToken;

enum SilikoTokenStatus
{
	SilikoTokenUnset = 0,
	SilikoTokenInteger,
	SilikoTokenReal,
	SilikoTokenCharacter,
	SilikoTokenId,
	SilikoTokenEndOfInput
};

SILIKO_EXPORT_C_API

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreate(void);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreateFromInteger(long long int source);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreateFromReal(double source);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreateFromCharacter(char source);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreateFromId(const char *source);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCreateEndOfInput(void);

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenCopy(const SilikoToken *source);

SILIKOCORE_EXPORT
void SilikoTokenDestroy(SilikoToken *object);


SILIKOCORE_EXPORT
void SilikoTokenAssignFromInteger(
	SilikoToken *object,
	long long int right_side);

SILIKOCORE_EXPORT
void SilikoTokenAssignFromReal(
	SilikoToken *object,
	double right_side);

SILIKOCORE_EXPORT
void SilikoTokenAssignFromCharacter(
	SilikoToken *object,
	char right_side);

SILIKOCORE_EXPORT
void SilikoTokenAssignFromId(
	SilikoToken *object,
	const char *right_side);

SILIKOCORE_EXPORT
void SilikoTokenAssignEndOfInput(
	SilikoToken *object);

SILIKOCORE_EXPORT
void SilikoTokenAssign(
	SilikoToken *object,
	const SilikoToken *right_side);


SILIKOCORE_EXPORT
enum SilikoTokenStatus SilikoTokenGetStatus(
	const SilikoToken *object);

SILIKOCORE_EXPORT
long long int SilikoTokenGetInteger(const SilikoToken *object);

SILIKOCORE_EXPORT
double SilikoTokenGetReal(const SilikoToken *object);

SILIKOCORE_EXPORT
char SilikoTokenGetCharacter(const SilikoToken *object);

SILIKOCORE_EXPORT
const char *SilikoTokenGetId(const SilikoToken *object);

SILIKO_END_C_API

#endif // SILIKO_CORE_TOKEN_H
