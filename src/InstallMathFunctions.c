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
#include <SilikoCore/Math.h>

int SilikoInstallMathFunctions(SilikoEngine *engine)
{
	if (!engine)
		return 0;

	return SilikoEngineInstallFunction(engine, "abs",
			SilikoPureFunctionCreate(SilikoMathAbs))
		&& SilikoEngineInstallFunction(engine, "acos",
			SilikoPureFunctionCreate(SilikoMathAcos))
		&& SilikoEngineInstallFunction(engine, "asin",
			SilikoPureFunctionCreate(SilikoMathAsin))
		&& SilikoEngineInstallFunction(engine, "atan",
			SilikoPureFunctionCreate(SilikoMathAtan))
		&& SilikoEngineInstallFunction(engine, "cbrt",
			SilikoPureFunctionCreate(SilikoMathCbrt))
		&& SilikoEngineInstallFunction(engine, "ceil",
			SilikoPureFunctionCreate(SilikoMathCeil))
		&& SilikoEngineInstallFunction(engine, "cos",
			SilikoPureFunctionCreate(SilikoMathCos))
		&& SilikoEngineInstallFunction(engine, "cosh",
			SilikoPureFunctionCreate(SilikoMathCosh))
		&& SilikoEngineInstallFunction(engine, "exp",
			SilikoPureFunctionCreate(SilikoMathExp))
		&& SilikoEngineInstallFunction(engine, "floor",
			SilikoPureFunctionCreate(SilikoMathFloor))
		&& SilikoEngineInstallFunction(engine, "hypot",
			SilikoPureFunctionCreate(SilikoMathHypot))
		&& SilikoEngineInstallFunction(engine, "log",
			SilikoPureFunctionCreate(SilikoMathLog))
		&& SilikoEngineInstallFunction(engine, "log2",
			SilikoPureFunctionCreate(SilikoMathLog2))
		&& SilikoEngineInstallFunction(engine, "log10",
			SilikoPureFunctionCreate(SilikoMathLog10))
		&& SilikoEngineInstallFunction(engine, "round",
			SilikoPureFunctionCreate(SilikoMathRound))
		&& SilikoEngineInstallFunction(engine, "sin",
			SilikoPureFunctionCreate(SilikoMathSin))
		&& SilikoEngineInstallFunction(engine, "sinh",
			SilikoPureFunctionCreate(SilikoMathSinh))
		&& SilikoEngineInstallFunction(engine, "sqrt",
			SilikoPureFunctionCreate(SilikoMathSqrt))
		&& SilikoEngineInstallFunction(engine, "tan",
			SilikoPureFunctionCreate(SilikoMathTan))
		&& SilikoEngineInstallFunction(engine, "tanh",
			SilikoPureFunctionCreate(SilikoMathTanh))
		&& SilikoEngineInstallFunction(engine, "trunc",
			SilikoPureFunctionCreate(SilikoMathTrunc))
;
}
