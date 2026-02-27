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

#include<SilikoCore/Value.h>

struct SilikoValue
{
	enum SilikoValueStatus status;
	union
	{
        enum SilikoError error;
		long long int integer;
		double real;
	};
};

SilikoValue *SilikoValueCreate(void)
{
    SilikoValue *object = malloc(sizeof *object);
    if(!object)
        return NULL;

    SilikoValueAssignFromInteger(object, 0);
    return object;
}

SilikoValue *SilikoValueCreateFromError(enum SilikoError source)
{
    SilikoValue *object = malloc(sizeof *object);
    if(!object)
        return NULL;

    SilikoValueAssignFromError(object, source);
    return object;
}

SilikoValue *SilikoValueCreateFromInteger(long long int source)
{
    SilikoValue *object = malloc(sizeof *object);
    if(!object)
        return NULL;

    SilikoValueAssignFromInteger(object, source);
    return object;
}

SilikoValue *SilikoValueCreateFromReal(double source)
{
    SilikoValue *object = malloc(sizeof *object);
    if(!object)
        return NULL;

    SilikoValueAssignFromReal(object, source);
    return object;
}

SilikoValue *SilikoValueCopy(const SilikoValue *other)
{
    SilikoValue *object = malloc(sizeof *object);
    if(!object)
        return NULL;

    SilikoValueAssign(object, other);
    return object;
}

void SilikoValueDestroy(SilikoValue *object)
{
    free(object);
}

void SilikoValueAssignFromError(
    SilikoValue *object,
    enum SilikoError source)
{
    if (!object)
        return;

    object->status = SilikoValueError;
    object->error = source;
}

void SilikoValueAssignFromInteger(
    SilikoValue *object,
    long long int source)
{
    if (!object)
        return;

    object->status = SilikoValueInteger;
    object->integer = source;
}

void SilikoValueAssignFromReal(
    SilikoValue *object,
    double source)
{
    if (!object)
        return;

    object->status = SilikoValueReal;
    object->real= source;
}

void SilikoValueAssign(
    SilikoValue *object,
    const SilikoValue *other)
{
    if (!object || object == other)
        return;

    object->status = other->status;
    switch(object->status)
    {
    case SilikoValueError:
        object->error = other->error;
        break;
    case SilikoValueInteger:
        object->integer = other->integer;
        break;
    case SilikoValueReal:
        object->real = other->real;
        break;
    }
}

enum SilikoValueStatus SilikoValueGetStatus(const SilikoValue *object)
{
    if (!object)
        return SilikoValueError;

    return object->status;
}

enum SilikoError SilikoValueGetError(const SilikoValue *object)
{
    if (!object)
        return SilikoErrorNullObject;

    switch (object->status)
    {
    case SilikoValueError:
        return object->error;
    case SilikoValueInteger:
    case SilikoValueReal:
        return SilikoErrorNone;
    }
}

long long int SilikoValueGetInteger(const SilikoValue *object)
{
    if (!object)
        return 0;

    switch (object->status)
    {
    case SilikoValueError:
        return 0;
    case SilikoValueInteger:
        return object->integer;
    case SilikoValueReal:
        return (long long int)object->real;
    }
}

double SilikoValueGetReal(const SilikoValue *object)
{
    if (!object)
        return NAN;

    switch (object->status)
    {
    case SilikoValueError:
        return NAN;
    case SilikoValueInteger:
        return (double)object->integer;
    case SilikoValueReal:
        return object->real;
    }
}

void SilikoValueNegate(SilikoValue *object)
{
    if (!object)
        return;

    switch(object->status)
    {
    case SilikoValueInteger:
        object->integer *= -1;
        break;
    case SilikoValueReal:
        object->real *= -1.0;
        break;
    default:
        // Ignore. Silence warnings.
        break;
    }
}
