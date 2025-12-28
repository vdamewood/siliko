/* Input.h: Abstract interface for parser input stream
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

#if !defined SILIKO_CORE_INPUT_H
#define SILIKO_CORE_INPUT_H

#include <SilikoCore/Api.h>

#if defined __cplusplus
extern "C" {
#endif

struct SilikoInput;
typedef struct SilikoInput SilikoInput;

struct SILIKOCORE_EXPORT SilikoInputVTable
{
	int (*advanceVirt)(void *);
	char (*getCharacterVirt)(void *);
	void (*deleteVirt)(void *);
};

SILIKOCORE_EXPORT SilikoInput *SilikoInputNew(
	const struct SilikoInputVTable *source_table,
	void *source_state);

SILIKOCORE_EXPORT int SilikoInputAdvance(SilikoInput *object);
SILIKOCORE_EXPORT char SilikoInputGetCharacter(SilikoInput *object);
SILIKOCORE_EXPORT void SilikoInputDelete(SilikoInput *object);

#if defined __cplusplus
}
#endif

#endif // SILIKO_CORE_INPUT_H
