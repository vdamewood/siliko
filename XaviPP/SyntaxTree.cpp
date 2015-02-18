/*
 * SyntaxTree.cpp: Abstract syntax tree classes
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

// Visual Studio complains about strcpy without this.
#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <cstring>

#include "SyntaxTree.hpp"
#include "FunctionCaller.hpp"

Xavi::IntegerNode::IntegerNode(int NewValue)
{
	MyInteger = NewValue;
}

Xavi::Value Xavi::IntegerNode::Evaluate()
{
	return MyInteger;
}

void Xavi::IntegerNode::Negate()
{
	MyInteger *= -1;
}

Xavi::FloatNode::FloatNode(float NewValue)
{
	MyFloat = NewValue;
}

Xavi::Value Xavi::FloatNode::Evaluate()
{
	return MyFloat;
}

void Xavi::FloatNode::Negate()
{
	MyFloat *= -1.0;
}

/*Xavi::BranchNode::BranchNode()
{
	MyFunctionId = 0;
	MyChildren = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
}*/

Xavi::BranchNode::BranchNode(const char *NewId)
{
	MyId = new char[strlen(NewId) + 1];
	std::strcpy(MyId, NewId);
	MyChildren = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
}

Xavi::BranchNode::~BranchNode()
{
	delete[] MyId;
	for (std::list<SyntaxTreeNode*>::iterator i = MyChildren.begin(); i != MyChildren.end(); i++)
		delete *i;
}

/*void Xavi::BranchNode::SetId(const char *NewId)
{
}*/

Xavi::Value Xavi::BranchNode::Evaluate()
{
	Xavi::Value rVal;
	std::vector<Xavi::Value> Arguments;

	if (MyChildren.size())
	{
		std::list<Xavi::SyntaxTreeNode *>::iterator i;
		for (i = MyChildren.begin(); i != MyChildren.end(); i++)
		{
			Xavi::Value Current = (*i)->Evaluate();
			if (!Current.IsNumber())
				return Current;

			Arguments.push_back(Current);
		}
	}

	rVal = Xavi::FunctionCaller::Call(MyId, Arguments);
	if (IsNegated)
	{
		if (rVal.Status() == Value::INTEGER)
			rVal = rVal.Integer() * -1;
		else if (rVal.Status() == Value::FLOAT)
			rVal = rVal.Float() * (float)-1.0;
	}

	return rVal;
}

void Xavi::BranchNode::Negate()
{
	IsNegated = !IsNegated;
}

void Xavi::BranchNode::PushLeft(Xavi::SyntaxTreeNode *NewChild)
{
	MyChildren.push_front(NewChild);
}

void Xavi::BranchNode::PushRight(Xavi::SyntaxTreeNode *NewChild)
{
	MyChildren.push_back(NewChild);
}


bool Xavi::BranchNode::GraftLeft(Xavi::SyntaxTreeNode *NewChild)
{
	if (MyChildren.size() == 0)
	{
		return false;
	}
	else if (MyChildren.front() == 0)
	{
		MyChildren.front() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyChildren.front()))
			return ChildBranch->GraftLeft(NewChild);
		else
			return false;
	}
}

bool Xavi::BranchNode::GraftRight(Xavi::SyntaxTreeNode *NewChild)
{
	if (MyChildren.size() == 0)
	{
		return false;
	}
	else if (MyChildren.back() == 0)
	{
		MyChildren.back() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyChildren.back()))
			return ChildBranch->GraftRight(NewChild);
		else
			return false;
	}
}

Xavi::SyntaxErrorNode::~SyntaxErrorNode(void)
{
}

Xavi::Value Xavi::SyntaxErrorNode::Evaluate(void)
{
	return Xavi::Value::SYNTAX_ERR;
}

void Xavi::SyntaxErrorNode::Negate(void)
{
}

Xavi::NothingNode::~NothingNode(void)
{
	// Do Nothing
}

Xavi::Value Xavi::NothingNode::Evaluate(void)
{
	return 0;
}

void Xavi::NothingNode::Negate(void)
{
	// Do Nothing
}
