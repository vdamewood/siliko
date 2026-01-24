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


#include <SilikoCore/Token.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#if defined _MSC_VER
#define strdup _strdup
#endif

struct SilikoToken
{
    enum SilikoTokenStatus status;
    union
    {
        char character;
        long long int integer;
        double real;
        char *id;
    };
};

SilikoToken *SilikoTokenCreate(void)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenUnset;
    object->integer = 0LL;
    return object;
}

SilikoToken *SilikoTokenCreateFromInteger(long long int source)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenInteger;
    object->integer = source;
    return object;
}

SilikoToken *SilikoTokenCreateFromReal(double source)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenReal;
    object->real = source;
    return object;
}

SilikoToken *SilikoTokenCreateFromCharacter(char source)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenCharacter;
    object->character = source;
    return object;
}

SilikoToken *SilikoTokenCreateFromId(const char *source)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenId;
    object->id = strdup(source);
    if (!object->id)
    {
        free(object);
        return NULL;
    }
    return object;
}

SilikoToken *SilikoTokenCreateEndOfInput(void)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = SilikoTokenEndOfInput;
    object->integer = 0LL;
    return object;
}

SilikoToken *SilikoTokenCopy(const SilikoToken *source)
{
    SilikoToken *object = malloc(sizeof(*object));
    if (!object)
        return NULL;
    object->status = source->status;
    switch (object->status)
    {
    case SilikoTokenCharacter:
        object->character = source->character;
        break;
    case SilikoTokenInteger:
        object->integer = source->integer;
        break;
    case SilikoTokenReal:
        object->real = source->real;
        break;
    case SilikoTokenId:
        object->id = strdup(source->id);
        break;
    case SilikoTokenUnset:
        object->integer = 0LL;
        break;
    case SilikoTokenEndOfInput:
        object->integer = 0LL;
        break;
    }
    return object;
}

void SilikoTokenDestroy(SilikoToken *object)
{
    if (object)
    {
        if (object->status == SilikoTokenId)
            free(object->id);
        free(object);
    }
}

void SilikoTokenAssignFromInteger(SilikoToken *object, long long int source)
{
    if (!object)
        return;

    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = SilikoTokenInteger;
    object->integer = source;
}

void SilikoTokenAssignFromReal(SilikoToken *object, double source)
{
    if (!object)
        return;

    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = SilikoTokenReal;
    object->real = source;
}

void SilikoTokenAssignFromCharacter(SilikoToken *object, char source)
{
    if (!object)
        return;

    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = SilikoTokenCharacter;
    object->character = source;
}

void SilikoTokenAssignFromId(SilikoToken *object, const char *source)
{
    if (!object)
        return;

    char * id_copy = strdup(source);
    if (!id_copy)
        return;
    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = SilikoTokenId;
    object->id = id_copy;
}

void SilikoTokenAssignEndOfInput(SilikoToken *object)
{
    if (!object)
        return;

    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = SilikoTokenEndOfInput;
    object->integer = 0;
}

void SilikoTokenAssign(SilikoToken *object, const SilikoToken *source)
{
    if (!object || object == source)
        return;

    if (object->status == SilikoTokenId)
        free(object->id);
    object->status = source->status;
        switch (object->status)
    {
    case SilikoTokenCharacter:
        object->character = source->character;
        break;
    case SilikoTokenInteger:
        object->integer = source->integer;
        break;
    case SilikoTokenReal:
        object->real = source->real;
        break;
    case SilikoTokenId:
        object->id = strdup(source->id);
        break;
    case SilikoTokenUnset:
        object->integer = 0LL;
        break;
    case SilikoTokenEndOfInput:
        object->integer = 0LL;
        break;
    }
}


enum SilikoTokenStatus SilikoTokenGetStatus(const SilikoToken *object)
{
    if (!object)
        return SilikoTokenUnset;

    return object->status;
}

long long int SilikoTokenGetInteger(const SilikoToken *object)
{
    if (!object || object->status != SilikoTokenInteger)
        return 0LL;

    return object->integer;
}

double SilikoTokenGetReal(const SilikoToken *object)
{
    if (!object || object->status != SilikoTokenReal)
        return NAN;

    return object->real;
}

char SilikoTokenGetCharacter(const SilikoToken *object)
{
    if (!object || object->status != SilikoTokenCharacter)
        return 0x7F;

    return object->character;
}

const char *SilikoTokenGetId(const SilikoToken *object)
{
    if (!object || object->status != SilikoTokenId)
        return NULL;

    return object->id;

}
