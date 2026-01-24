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

// Based on Mersenne Twister developed by Makoto Matsumoto and
// Takuji Nishimura <https://dl.acm.org/doi/10.1145/272991.272995>
// with improvements from
// <https://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html> and
// <https://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt64.html>.

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SilikoCore/Function.h>
#include <SilikoCore/Operation.h>

// Mersenne twister parameters. Table size has to be a #define because
// it's used in an array declaration.
#define TABLE_SIZE 312
static const          long long int TableSize  = TABLE_SIZE;
static const          long long int TableBreak = 156;
static const unsigned long long int MatrixA    = 0xb5026f5aa96619e9ULL;
static const unsigned long long int UpperMask  = 0xffffffff80000000ULL;
static const unsigned long long int LowerMask  = 0x000000007fffffffULL;

// Seeding parameters
static const          long long int Multiplier = 6364136223846793005ULL;
static const unsigned long long int SeedShift  = 62ULL;
static const unsigned long long int TimeMask   = 0xffffffffffffffffULL;

static inline unsigned long long int Temper(
	unsigned long long int x)
{
	x ^= (x >> 29) & 0x5555555555555555; // ShiftU & MaskD
	x ^= (x << 17) & 0x71D67FFFEDA60000; // ShiftS & MaskB
	x ^= (x << 37) & 0xFFF7EEE000000000; // ShiftT & MaskC
	x ^= (x >> 43);                      // ShiftL
	return x;
}

static inline unsigned long long int Twist(
	unsigned long long int u,
	unsigned long long int v)
{
	return ((((u & UpperMask) | (v & LowerMask)) >> 1)
		^ (v&1 ? MatrixA : 0));
}

static SilikoValue *call(void *void_state, int argc, SilikoValue **argv);
static void destroy(void *object);

static const struct SilikoFunctionVTable OperationDiceVTable = {
    call,
    destroy
};

struct DiceState
{
	unsigned long long int Table[TABLE_SIZE];
	size_t Index;
};

static unsigned long long int genrand(struct DiceState *object)
{
	if (object->Index >= TableSize)
	{
		int i = 0;
		for(; i < (TableSize - TableBreak); i++)
			object->Table[i] = object->Table[i+TableBreak]
				^ Twist(object->Table[i], object->Table[i+1]);
		for(; i < (TableSize - 1); i++)
			object->Table[i] = object->Table[i+TableBreak-TableSize]
				^ Twist(object->Table[i], object->Table[i+1]);
		object->Table[i] = object->Table[TableBreak-1]
				^ Twist(object->Table[i], object->Table[0]);

		object->Index = 0;
	}

	return Temper(object->Table[object->Index++]);
}

static SilikoValue *call(void *void_state, int argc, SilikoValue **argv)
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

	long long int result = 0;
	for (int i = 1; i <= count; i++)
		result += (genrand(void_state) % faces) + 1;
	return SilikoValueCreateFromInteger(result);
}


static void destroy(void *object)
{
	free(object);
}

SilikoFunction *SilikoOperationDiceCreate(unsigned long long int seed)
{
	struct DiceState *state = malloc(sizeof *state);
	if (!state)
		return NULL;

	if (seed == 0)
		seed = ((unsigned long long int)time(NULL)) & TimeMask;

	state->Table[0] = seed;
	for(int i = 1; i < TableSize; i++)
		state->Table[i]
			= Multiplier
			* (state->Table[i-1]
				^ (state->Table[i-1] >> SeedShift)) + i;
	state->Index = TableSize;

    SilikoFunction *object =
        SilikoFunctionCreate(&OperationDiceVTable, state);
    if (!object)
        destroy(state);
	return object;
}
