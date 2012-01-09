/*
 * XvcState.h: State information.
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
void XvcStateClear(void);
void XvcStateSetNil(void);
void XvcStateSetValue(XvcNumber);
void XvcStateSetOutOfMemoryError(void);

#endif // XVC_STATE_H