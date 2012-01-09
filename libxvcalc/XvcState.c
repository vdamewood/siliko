/*
 * XvcState.c: State information.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of XVCalc.
 *
 * XVCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * XVCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with XVCalc. If not, see <http://www.gnu.org/licenses/>.
 */

#include "XvcStructs.h"
#include "XvcState.h"

static XvcStatus Status = NONE;
static int       ValueAsInteger = 0;
static float     ValueAsFloat   = 0.0;

XvcStatus XvcStateStatus()
{
	return Status;
}

void XvcStateSetStatus(XvcStatus NewStatus)
{
	Status = NewStatus;
}

int XvcStateInteger()
{
	return ValueAsInteger;
}

void XvcStateSetInteger(int NewInteger)
{
	ValueAsInteger = NewInteger;
}

float XvcStateFloat()
{
	return ValueAsFloat;
}

void XvcStateSetFloat(float NewFloat)
{
	ValueAsFloat = NewFloat;
}

void XvcStateClear(void)
{
	XvcStateSetStatus(NONE);
	XvcStateSetInteger(0);
	XvcStateSetFloat(0.0);
}

void XvcStateSetNil(void)
{
	Status = S_INTEGER;
	ValueAsInteger = 0;
	ValueAsFloat = 0.0;
}

void XvcStateSetValue(XvcNumber NewNumber)
{
	switch (NewNumber.type) {
		case 'i':
			Status = S_INTEGER;
			ValueAsInteger = NewNumber.i;
			ValueAsFloat = (float) NewNumber.i;
			break;
		case 'f':
			Status = S_FLOAT;
			ValueAsInteger = (int) NewNumber.f;
			ValueAsFloat = NewNumber.f;
	};
}

void XvcStateSetOutOfMemoryError(void)
{
	Status = E_MEMORY;
}
