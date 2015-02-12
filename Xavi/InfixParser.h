/*
 * XaviParser.h: Parser for Xavi
 * Copyright 2014, 2015 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined XAVI_INFIX_PARSER_H
#define XAVI_INFIX_PARSER_H

#include "W32Dll.h"
#include "DataSource.h"
#include "SyntaxTree.h"

XAVI_API XaviSyntaxTreeNode *XaviParseInfix(XaviDataSource *input);

#endif /* XAVI_INFIX_PARSER_H */
