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

void XaviCleanupCacheTree(XaviTree * TreeToCache);
void XaviCleanupReleaseTree(XaviTree * TreeToRelease);
void XaviCleanupClearTrees();
void XaviCleanupCacheArglist(XaviArglist * ArglistToCache);
void XaviCleanupReleaseArglist(XaviArglist * ArglistToRelease);
void XaviCleanupClearArglists();
void XaviCleanupCacheFunctionId(char * FunctionIdToCache);
void XaviCleanupReleaseFunctionId(char * FunctionIdToRelease);
void XaviCleanupClearFunctionIds();
void XaviCleanupClearAll();

#endif // Xavi_CLEANUP_H