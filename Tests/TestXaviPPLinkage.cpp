/*
 * TestXaviPPLinkage.cpp: Program to test public linkage
 * Copyright 2015 Vincent Damewood
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

#include <XaviPP/W32Dll.hpp>
#include <XaviPP/Value.hpp>
#include <XaviPP/FunctionCaller.hpp>
#include <XaviPP/SyntaxTree.hpp>
#include <XaviPP/DataSource.hpp>
#include <XaviPP/CStringSource.hpp>
#include <XaviPP/Token.hpp>
#include <XaviPP/Lexer.hpp>
#include <XaviPP/InfixParser.hpp>

using namespace Xavi;

Value FakeFunction(std::vector<Value> args)
{
	Value rVal;
	return rVal;
}

int main(int argc, char *argv[])
{
	Value FakeBlankValue;
	Value FakeStatusValue(Value::SYNTAX_ERR);
	Value FakeIntValue(0);
	Value FakeFloatValue((float)0.0);
	FakeBlankValue.Status();
	FakeIntValue.Integer();
	FakeFloatValue.Float();
	FakeIntValue.IsNumber();
	FakeStatusValue.ToCString();
	FakeBlankValue.~Value();

	// This might need to be fixed. Change std::vector to pointer.
	std::vector<Value> FakeArgs;
	FunctionCaller::SetUp();
	FunctionCaller::TearDown();
	FunctionCaller::Call("", FakeArgs);
	FunctionCaller::Install("", FakeFunction);

	SyntaxTreeNode *FakeNode = NULL;
	FakeNode->Evaluate();
	FakeNode->Negate();
	FakeNode->~SyntaxTreeNode();

	IntegerNode FakeIntegerNode(0);
	FakeIntegerNode.Evaluate();
	FakeIntegerNode.Negate();
	FakeIntegerNode.~IntegerNode();

	FloatNode FakeFloatNode(0.0);
	FakeFloatNode.Evaluate();
	FakeFloatNode.Negate();
	FakeFloatNode.~FloatNode();


	BranchNode FakeBranchNode("");
	FakeBranchNode.Evaluate();
	FakeBranchNode.Negate();

	FakeBranchNode.PushLeft(FakeNode);
	FakeBranchNode.PushRight(FakeNode);
	FakeBranchNode.GraftLeft(FakeNode);
	FakeBranchNode.GraftRight(FakeNode);
	FakeBranchNode.~BranchNode();

	SyntaxErrorNode FakeErrorNode;
	FakeErrorNode.Evaluate();
	FakeErrorNode.Negate();
	FakeErrorNode.~SyntaxErrorNode();

	NothingNode FakeNothingNode;
	FakeNothingNode.Evaluate();
	FakeNothingNode.Negate();
	FakeNothingNode.~NothingNode();

	DataSource *FakeSource = NULL;
	FakeSource->~DataSource();
	FakeSource->Advance();
	FakeSource->GetCurrent();

	CStringSource FakeStringSource("");
	FakeStringSource.Advance();
	FakeStringSource.GetCurrent();
	FakeStringSource.~CStringSource();

	Token FakeErrorToken(Token::ERROR);
	Token FakeIntegerToken(0);
	Token FakeFloatToken((float)0.0);
	Token FakeIdToken("");
	FakeErrorToken.~Token();

	FakeErrorToken.Type();
	FakeIntegerToken.Integer();
	FakeFloatToken.Float();
	FakeIdToken.Id();

	Lexer FakeLexer(FakeSource);
	FakeLexer.~Lexer();

	FakeLexer.GetToken();
	FakeLexer.Next();

	ParseInfix(FakeSource);
	return 255;
}
