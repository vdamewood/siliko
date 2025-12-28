/* Value.c: Data structure to represent a single value
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

#include <math.h>

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

SilikoValue *SilikoValueNewFromError(enum SilikoError source)
{
    SilikoValue *object = malloc(sizeof (*object));
    if(!object)
        return NULL;

    SilikoValueAssignError(object, source);
    return object;
}

SilikoValue *SilikoValueNewFromInteger(long long int source)
{
    SilikoValue *object = malloc(sizeof (*object));
    if(!object)
        return NULL;

    SilikoValueAssignInteger(object, source);
    return object;
}

SilikoValue *SilikoValueNewFromReal(double source)
{
    SilikoValue *object = malloc(sizeof (*object));
    if(!object)
        return NULL;

    SilikoValueAssignReal(object, source);
    return object;
}

SilikoValue *SilikoValueNewCopy(const SilikoValue *other)
{
    SilikoValue *object = malloc(sizeof (*object));
    if(!object)
        return NULL;

    SilikoValueCopy(object, other);
    return object;
}

void SilikoValueDelete(SilikoValue *object)
{
    free(object);
}

enum SilikoError SilikoValueGetError(const SilikoValue *object)
{
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

void SilikoValueAssignError(
    SilikoValue *object,
    enum SilikoError source)
{
    object->status = SilikoValueError;
    object->error = source;
}

void SilikoValueAssignInteger(
    SilikoValue *object,
    long long int source)
{
    object->status = SilikoValueInteger;
    object->integer = source;
}

void SilikoValueAssignReal(
    SilikoValue *object,
    double source)
{
    object->status = SilikoValueReal;
    object->real= source;
}

void SilikoValueCopy(
    SilikoValue *object,
    const SilikoValue *other)
{
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

void SilikoValueNegate(SilikoValue *object)
{
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

enum SilikoValueStatus SilikoValueGetStatus(const SilikoValue *object)
{
    return object->status;
}

