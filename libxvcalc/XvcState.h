#if !defined XVC_STATE_H
#define XVC_STATE_H

#include "XVCalc.h"
#include "XvcStructs.h"

XvcStatus XvcStateStatus(void);
void XvcStateSetStatus(XvcStatus NewStatus);
int XvcStateInteger(void);
void XvcStateSetInteger(int NewInteger);
float XvcStateFloat(void);
void XvcStateSetFloat(float NewFloat);
void XvcStateSetNil(void);
void XvcStateSetValue(XvcNumber);
void XvcStateSetOutOfMemoryError(void);

#endif // XVC_STATE_H