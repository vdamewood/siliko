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
		SilikoTokenUnset,
		SilikoTokenInteger,
		SilikoTokenReal,
		SilikoTokenCharacter,
		SilikoTokenId,
		SilikoTokenEndOfInput
};

#if defined __cplusplus
extern "C" {
#endif

SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNew(void);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewFromInteger(long long int source);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewFromReal(double source);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewFromCharacter(char source);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewFromId(const char *source);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewEndOfInput(void);
SILIKOCORE_EXPORT
SilikoToken *SilikoTokenNewCopy(const SilikoToken *source);
SILIKOCORE_EXPORT
void SilikoTokenDelete(SilikoToken *object);

SILIKOCORE_EXPORT
void SilikoTokenAssignInteger(
	SilikoToken *object,
	long long int source);
SILIKOCORE_EXPORT
void SilikoTokenAssignReal(SilikoToken *object, double source);
SILIKOCORE_EXPORT
void SilikoTokenAssignCharacter(SilikoToken *object, char source);
SILIKOCORE_EXPORT
void SilikoTokenAssignId(SilikoToken *object, const char *source);
SILIKOCORE_EXPORT
void SilikoTokenAssignEndOfInput(SilikoToken *object);
SILIKOCORE_EXPORT
void SilikoTokenCopy(
	SilikoToken *object,
	const SilikoToken *source);

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

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_TOKEN_H
