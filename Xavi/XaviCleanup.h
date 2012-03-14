/*
 * XaviEvaluate.h: Keep track of dangling objects while composing syntax tree.
 * Copyright 2012 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined Xavi_CLEANUP_H
#define Xavi_CLEANUP_H

#include "XaviStructs.h"

int XaviCleanupCacheTree(XaviTree * TreeToCache, XaviMemoryPool *);
void XaviCleanupReleaseTree(XaviTree * TreeToRelease, XaviMemoryPool *);
void XaviCleanupClearTrees(XaviMemoryPool *);
int XaviCleanupCacheArglist(XaviArglist * ArglistToCache, XaviMemoryPool *);
void XaviCleanupReleaseArglist(XaviArglist * ArglistToRelease, XaviMemoryPool *);
void XaviCleanupClearArglists(XaviMemoryPool *);
int XaviCleanupCacheFunctionId(char * FunctionIdToCache, XaviMemoryPool *);
void XaviCleanupReleaseFunctionId(char * FunctionIdToRelease, XaviMemoryPool *);
void XaviCleanupClearFunctionIds(XaviMemoryPool *);
void XaviCleanupClearAll(XaviMemoryPool *);

#endif // Xavi_CLEANUP_H