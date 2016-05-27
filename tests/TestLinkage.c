/* TestSilikoLinkage.c: Program to test public linkage
 * Copyright 2015, 2016 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "W32Dll.h"
#include "Value.h"
#include "FunctionCaller.h"
#include "SyntaxTree.h"
#include "DataSource.h"
#include "StringSource.h"
#include "Token.h"
#include "Lexer.h"
#include "InfixParser.h"

SilikoValue FakeFunction(int argc, SilikoValue *argv)
{
	SilikoValue rVal;
	return rVal;
}

int main(int argc, char *argv[])
{
	SilikoDataSource *FakeSource = NULL;
	SilikoValue *FakeValueList = NULL;
	SilikoLexer *FakeLexer = NULL;
	SilikoSyntaxTreeNode *FakeNode = NULL;
	SilikoValue FakeValue;

	SilikoDataSourceAdvance(FakeSource);
	SilikoDataSourceGet(FakeSource);
	SilikoDataSourceDelete(FakeSource);
	SilikoFunctionCallerSetUp();
	SilikoFunctionCallerTearDown();
	SilikoFunctionCallerCall("fake", 1, FakeValueList);
	SilikoFunctionCallerInstall("fake", FakeFunction);
	SilikoParseInfix(FakeSource);

	SilikoLexerNew(FakeSource);
	SilikoLexerDelete(FakeLexer);
	SilikoLexerNext(FakeLexer);
	SilikoStringSourceNew("");
	SilikoSyntaxTreeNewInteger(0);
	SilikoSyntaxTreeNewFloat(0.0);
	SilikoSyntaxTreeNewBranch("");
	SilikoSyntaxTreeNewError();
	SilikoSyntaxTreeNewNothing();
	SilikoSyntaxTreeDelete(FakeNode);
	SilikoSyntaxTreeEvaluate(FakeNode);
	SilikoSyntaxTreeNegate(FakeNode);
	SilikoSyntaxTreePushLeft(FakeNode, FakeNode);
	SilikoSyntaxTreePushRight(FakeNode, FakeNode);
	SilikoSyntaxTreeGraftLeft(FakeNode, FakeNode);
	SilikoSyntaxTreeGraftRight(FakeNode, FakeNode);
	SilikoValueToString(FakeValue);
	return 255;
}
