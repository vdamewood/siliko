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


#include <stdlib.h>

#include <SilikoCore/Engine.h>
#include <SilikoCore/Evaluate.h>
#include <SilikoCore/Node.h>
#include <SilikoCore/Value.h>

SilikoValue *SilikoEvaluate(
    SilikoEngine *engine,
    const SilikoNode *node)
{
	if (!node || !engine)
        return SilikoValueCreateFromError(SilikoErrorNullObject);

	switch (SilikoNodeGetStatus(node))
	{
	case SilikoNodeLeaf:
		return SilikoValueCopy(SilikoNodeGetValue(node));
	case SilikoNodeBranch:
    {
        SilikoValue **args = NULL;

        int child_count = SilikoNodeCountChildren(node);
        if (child_count)
        {
            if (!(args = calloc(child_count, sizeof(*args))))
                return NULL;

            for(int i = 0; i < child_count; i++)
            {
                args[i] = SilikoEvaluate(
                    engine,
                    SilikoNodeFetchChild(node, i));
                if(SilikoValueGetStatus(args[i])
                    == SilikoValueError)
                {
                    SilikoValue *error = args[i];
                    args[i] = NULL;
                    for (int j = 0; j < i; j++)
                        SilikoValueDestroy(args[j]);
                    free(args);
                    return error;
                }
            }
        }

        SilikoValue *result
            = SilikoEngineCallFunction(
                engine, SilikoNodeGetId(node), child_count, args);
        free(args);

        if (SilikoNodeIsNegated(node))
            SilikoValueNegate(result);
        return result;
    }
    default:
        return SilikoValueCreateFromError(SilikoErrorSyntax);
	}
}
