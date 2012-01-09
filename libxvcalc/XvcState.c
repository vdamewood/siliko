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
