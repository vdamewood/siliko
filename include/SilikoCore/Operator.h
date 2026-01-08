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


#if !defined SILIKO_OPERATORS_H
#define SILIKO_OPERATORS_H

#include <SilikoCore/Api.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Value.h>

SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorAdd(int argc, SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorSubtract(int argc, SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorMultiply(int argc, SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorDivide(int argc, SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorPower(int argc, SilikoValue **argv);
SILIKOCORE_EXPORT
SilikoValue *SilikoOperatorDice(int argc, SilikoValue **argv);

SILIKOCORE_EXPORT
int SilikoInstallOperators(SilikoEngine *destination);

#endif // SILIKO_OPERATORS_H
