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


#if !defined SILIKO_CORE_MATH_H
#define SILIKO_CORE_MATH_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Value.h>

SILIKO_EXPORT_C_API

SILIKOCORE_EXPORT
SilikoValue *SilikoMathAbs(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathAcos(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathAsin(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathAtan(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathCbrt(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathCeil(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathCos(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathCosh(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathExp(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathFloor(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathHypot(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathLog(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathLog2(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathLog10(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathRound(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathSin(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathSinh(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathSqrt(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathTan(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathTanh(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
SilikoValue *SilikoMathTrunc(int argc, SilikoValue **argv);


SILIKOCORE_EXPORT
bool SilikoInstallMathFunctions(SilikoEngine *destination);

SILIKO_END_C_API

#endif // SILIKO_CORE_MATH_H
