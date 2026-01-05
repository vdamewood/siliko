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


#include <SilikoCore/Evaluate.h>
#include <SilikoCore/Value.h>

SilikoValue *SilikoEvaluateNode(const SilikoNode *node, SilikoEngine *engine)
{
	if (!node)
		return SilikoValueNewFromError(SilikoErrorSyntax);

	switch (SilikoNodeGetStatus(node))
	{
	case SilikoNodeLeaf:
		return SilikoValueNewCopy(SilikoNodeGetValue(node));
	case SilikoNodeBranch:
    {
        SilikoValue **Arguments = NULL;

        int child_count = SilikoNodeCountChildren(node);
        if (child_count)
        {
            if (!(Arguments = calloc(child_count, sizeof(*Arguments))))
                return NULL;

            for(int i = 0; i < child_count; i++)
            {
                Arguments[i] = SilikoEvaluateNode(
                    SilikoNodeFetchChild(node, i), engine);
                if(SilikoValueGetStatus(Arguments[i])
                    == SilikoValueError)
                {
                    SilikoValue *error = Arguments[i];
                    Arguments[i] = NULL;
                    for (int j = 0; j < i; j++)
                        SilikoValueDelete(Arguments[j]);
                    free(Arguments);
                    return error;
                }
            }
        }

        SilikoValue *result
            = SilikoEngineCallFunction(
                engine, SilikoNodeGetId(node), child_count, Arguments);
        free(Arguments);

        if (SilikoNodeIsNegated(node))
            SilikoValueNegate(result);
        return result;    }
        default: // Shouldn't happen, but just in case.
            return SilikoValueNewFromError(SilikoErrorSyntax);
	}
}
