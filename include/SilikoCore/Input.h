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


#if !defined SILIKO_CORE_INPUT_H
#define SILIKO_CORE_INPUT_H

#include <SilikoCore/Api.h>

typedef struct SilikoInput SilikoInput;

SILIKO_EXPORT_C_API

struct SilikoInputVTable
{
	int (*advance)(void *state);
	char (*getCharacter)(void *state);
	void (*destroy)(void *state);
};

SILIKOCORE_EXPORT
SilikoInput *SilikoInputCreate(
	const struct SilikoInputVTable *source_table,
	void *source_state);

SILIKOCORE_EXPORT
void SilikoInputDestroy(SilikoInput *object);


SILIKOCORE_EXPORT
int SilikoInputAdvance(SilikoInput *object);

SILIKOCORE_EXPORT
char SilikoInputGetCharacter(SilikoInput *object);

SILIKOCORE_EXPORT
SilikoInput *SilikoStringInputCreate(const char *input_string);


SILIKO_END_C_API

#endif // SILIKO_CORE_INPUT_H
