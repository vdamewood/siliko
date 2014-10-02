/*
 * InfixParser.hpp: Recursive-descent infix parser.
 * Copyright 2014 Vincent Damewood
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

#if !defined XAVI_INFIX_PARSER_H
#define XAVI_INFIX_PARSER_H

#include "SyntaxTree.hpp"
#include "Lexer.hpp"

#if !defined API
#  if defined _WIN32 && defined _MSC_VER
#    if defined Xavi_EXPORTS
#      define API
#    else Xavi_EXPORTS
#      define API __declspec(dllimport)
#    endif // Xavi_EXPORTS
#  else // _WIN32 && __MSC_VER
#    define API
#  endif // _WIN32 && __MSC_VER
#endif // API

namespace Xavi
{
	class InfixParser
	{
	public:
		InfixParser(Xavi::Lexer *NewLexer);
		~InfixParser(void);
		void Parse(void);
		SyntaxTreeNode &SyntaxTree(void);
	private:
		Xavi::SyntaxTreeNode *GetExpr0(void);
		Xavi::BranchNode     *GetExpr0r(void);
		Xavi::SyntaxTreeNode *GetExpr1(void);
		Xavi::BranchNode     *GetExpr1r(void);
		Xavi::SyntaxTreeNode *GetExpr2(void);
		Xavi::SyntaxTreeNode *GetExpr2lf(void);
		Xavi::SyntaxTreeNode *GetExpr3(void);
		Xavi::SyntaxTreeNode *GetExpr3lf(void);
		Xavi::SyntaxTreeNode *GetAtom(void);
		Xavi::SyntaxTreeNode *GetNumber(void);
		Xavi::SyntaxTreeNode *GetUNumber(void);
		Xavi::SyntaxTreeNode *GetFCall(void);
		Xavi::BranchNode     *GetArguments(void);

		Lexer *MyLexer;
		SyntaxTreeNode *MySyntaxTree;
	};
};
#endif // XAVI_INFIX_PARSER_H
