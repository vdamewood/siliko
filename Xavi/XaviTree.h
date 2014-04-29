/*
 * XaviTree.h: Functions to manipulate abstract syntax trees.
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

#if !defined Xavi_TREE_H
#define Xavi_TREE_H

#include "XaviStructs.h"

int XaviTreeGraftLeft(XaviTree * parent, XaviTree * left, XaviMemoryPool *);
XaviTree * XaviTreeNewOperator(XaviOperatorSymbol Operator, XaviTree * LeftOperand, XaviTree * RightOperand, XaviMemoryPool *);
XaviTree * XaviTreeNewInteger(int Value, XaviMemoryPool *);
XaviTree * XaviTreeNewFloat(float Value, XaviMemoryPool *);
XaviTree * XaviTreeNewFunction(char * FunctionName, XaviArglist * FunctionArguments, XaviMemoryPool *);
void XaviTreeDelete(XaviTree * TreeToDelete);

#endif // Xavi_TREE_H
