/*
 * TestXaviLinkage.c: Program to test public linkage
 * Copyright 2012, 2015 Vincent Damewood
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

#include <Xavi/W32Dll.h>
#include <Xavi/Value.h>
#include <Xavi/FunctionCaller.h>
#include <Xavi/SyntaxTree.h>
#include <Xavi/DataSource.h>
#include <Xavi/StringSource.h>
#include <Xavi/Lexer.h>
#include <Xavi/InfixParser.h>

XaviValue FakeFunction(int argc, XaviValue *argv)
{
	XaviValue rVal;
	return rVal;
}

int main(int argc, char *argv[])
{
	XaviDataSource *FakeSource = NULL;
	XaviValue *FakeValueList = NULL;
	XaviLexer *FakeLexer = NULL;
	XaviSyntaxTreeNode *FakeNode = NULL;
	XaviValue FakeValue;

	XaviDataSourceAdvance(FakeSource);
	XaviDataSourceGet(FakeSource);
	XaviDataSourceDelete(FakeSource);
	XaviFunctionCallerSetUp();
	XaviFunctionCallerTearDown();
	XaviFunctionCallerCall("fake", 1, FakeValueList);
	XaviFunctionCallerInstall("fake", FakeFunction);
	XaviParseInfix(FakeSource);

	XaviLexerNew(FakeSource);
	XaviLexerDelete(FakeLexer);
	XaviLexerNext(FakeLexer);
	XaviStringSourceNew("");
	XaviSyntaxTreeNewInteger(0);
	XaviSyntaxTreeNewFloat(0.0);
	XaviSyntaxTreeNewBranch("");
	XaviSyntaxTreeNewError();
	XaviSyntaxTreeNewNothing();
	XaviSyntaxTreeDelete(FakeNode);
	XaviSyntaxTreeEvaluate(FakeNode);
	XaviSyntaxTreeNegate(FakeNode);
	XaviSyntaxTreePushLeft(FakeNode, FakeNode);
	XaviSyntaxTreePushRight(FakeNode, FakeNode);
	XaviSyntaxTreeGraftLeft(FakeNode, FakeNode);
	XaviSyntaxTreeGraftRight(FakeNode, FakeNode);
	XaviValueToString(FakeValue);
	return 255;
}
