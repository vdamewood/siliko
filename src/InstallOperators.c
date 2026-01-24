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


#include <SilikoCore/Engine.h>
#include <SilikoCore/Function.h>
#include <SilikoCore/Operator.h>

int SilikoInstallOperators(SilikoEngine *engine)
{
	if (!engine)
		return 0;

	return SilikoEngineInstallFunction(engine, "add",
			SilikoPureFunctionCreate(SilikoOperatorAdd))
		&& SilikoEngineInstallFunction(engine, "subtract",
			SilikoPureFunctionCreate(SilikoOperatorSubtract))
		&& SilikoEngineInstallFunction(engine, "multiply",
			SilikoPureFunctionCreate(SilikoOperatorMultiply))
		&& SilikoEngineInstallFunction(engine, "divide",
			SilikoPureFunctionCreate(SilikoOperatorDivide))
		&& SilikoEngineInstallFunction(engine, "power",
			SilikoPureFunctionCreate(SilikoOperatorPower))
		&& SilikoEngineInstallFunction(engine, "dice",
			SilikoOperatorDiceCreate(0));
}
