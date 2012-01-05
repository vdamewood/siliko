#if !defined XVC_STATE_H
#define XVC_STATE_H
#include "xvcalc.h"
#include "structs.h"

xvcalc_status XvcStateStatus();
void XvcStateSetStatus(xvcalc_status NewStatus);
int XvcStateInteger();
void XvcStateSetInteger(int NewInteger);
float XvcStateFloat();
void XvcStateSetFloat(float NewFloat);
const char * XvcStateErrorMessage();
void XvcStateSetErrorMessage(const char * newErrorMessage);

void  xvcalc_set_nil();
void  xvcalc_set_value(number);
void  xvcalc_set_malloc_error(void);

#endif