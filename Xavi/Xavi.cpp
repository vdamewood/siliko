/*
 * Xavi.c: Public interface for Xavi.
 * Copyright 2012, 2014 Vincent Damewood
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

#include "Xavi.h"
#include "FunctionCall.hpp"
#include "Parser.hpp"
#include "Value.hpp"
#include "StringSource.hpp"

void XaviOpen(void)
{
	Xavi::Functions::Open();
}

void XaviClose(void)
{
	Xavi::Functions::Close();
}

static XaviResult XaviValueToResult(const Xavi::Value value)
{
	XaviResult rVal;
	switch (value.Status())
	{
	case Xavi::Value::INTEGER:
		rVal.status = XAVI_RS_INTEGER;
		rVal.i = value.IntegerValue();
		break;
	case Xavi::Value::FLOAT:
		rVal.status = XAVI_RS_FLOAT;
		rVal.f = value.FloatValue();
		break;
	case Xavi::Value::MEMORY_ERR:
		rVal.status = XAVI_RE_MEMORY;
		rVal.i = 0;
		break;
	case Xavi::Value::SYNTAX_ERR:
		rVal.status = XAVI_RE_SYNTAX;
		rVal.i = 0;
		break;
	case Xavi::Value::ZERO_DIV_ERR:
		rVal.status = XAVI_RE_ZERO_DIV;
		rVal.i = 0;
		break;
	case Xavi::Value::BAD_FUNCTION:
		rVal.status = XAVI_RE_FUNCTION;
		rVal.i = 0;
		break;
	case Xavi::Value::BAD_ARGUMENTS:
		rVal.status = XAVI_RE_ARGUMENTS;
		rVal.i = 0;
		break;
	case Xavi::Value::DOMAIN_ERR:
		rVal.status = XAVI_RE_DOMAIN;
		rVal.i = 0;
		break;
	case Xavi::Value::RANGE_ERR:
		rVal.status = XAVI_RE_RANGE;
		rVal.i = 0;
		break;
	default:
		rVal.status = XAVI_RE_INTERNAL;
		rVal.i = 0;
		break;
	}

	return rVal;
}

XaviResult XaviEvaluate(const char *InputString)
{
	Xavi::InfixParser MyParser(new Xavi::Lexer(new Xavi::StringSource(InputString)));
	MyParser.Parse();
	return  XaviValueToResult(MyParser.SyntaxTree().GetValue());
}
