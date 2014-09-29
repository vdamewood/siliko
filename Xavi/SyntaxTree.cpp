/*
 * XaviTree.c: Functions to manipulate abstract syntax trees.
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

#include "SyntaxTree.hpp"
#include "FunctionCall.hpp"

Xavi::IntegerNode::IntegerNode(int NewValue)
{
	Value = NewValue;
}

Xavi::Value Xavi::IntegerNode::GetValue()
{
	return Value;
}

void Xavi::IntegerNode::Negate()
{
	Value *= -1;
}

Xavi::FloatNode::FloatNode(float NewValue)
{
	Value = NewValue;
}

Xavi::Value Xavi::FloatNode::GetValue()
{
	return Value;
}

void Xavi::FloatNode::Negate()
{
	Value *= -1.0;
}

Xavi::BranchNode::BranchNode(void)
{
	Children = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
}

Xavi::BranchNode::BranchNode(std::string NewId)
{
	Children = std::list<Xavi::SyntaxTreeNode *>();
	IsNegated = false;
	FunctionId = NewId;
}

Xavi::BranchNode::~BranchNode()
{
	for (std::list<SyntaxTreeNode*>::iterator i = Children.begin(); i != Children.end(); i++)
		delete *i;
}

void Xavi::BranchNode::SetId(std::string NewId)
{
	FunctionId = NewId;
}

Xavi::Value Xavi::BranchNode::GetValue()
{
	std::vector<Xavi::Value> Arguments;

	if (Children.size())
	{
		std::list<Xavi::SyntaxTreeNode *>::iterator i;
		for (i = Children.begin(); i != Children.end(); i++)
		{
			Xavi::Value Current = (*i)->GetValue();
			if (!Current.IsNumber())
				return Current;

			Arguments.push_back(Current);
		}
	}

	return Xavi::FunctionCaller::Default().Call(FunctionId, Arguments);
}

void Xavi::BranchNode::Negate()
{
	IsNegated = !IsNegated;
}

void Xavi::BranchNode::PushLeft(Xavi::SyntaxTreeNode *NewChild)
{
	Children.push_front(NewChild);
}

void Xavi::BranchNode::PushRight(Xavi::SyntaxTreeNode *NewChild)
{
	Children.push_back(NewChild);
}


bool Xavi::BranchNode::GraftLeft(Xavi::SyntaxTreeNode *NewChild)
{
	if (Children.size() == 0)
	{
		return false;
	}
	else if (Children.front() == 0)
	{
		Children.front() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(Children.front()))
			return ChildBranch->GraftLeft(NewChild);
		else
			return false;
	}
}

bool Xavi::BranchNode::GraftRight(Xavi::SyntaxTreeNode *NewChild)
{
	if (Children.size() == 0)
	{
		return false;
	}
	else if (Children.back() == 0)
	{
		Children.back() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(Children.back()))
			return ChildBranch->GraftRight(NewChild);
		else
			return false;
	}
}

Xavi::SyntaxErrorNode::~SyntaxErrorNode(void)
{
}

Xavi::Value Xavi::SyntaxErrorNode::GetValue(void)
{
	return Xavi::Value::SYNTAX_ERR;
}

void Xavi::SyntaxErrorNode::Negate(void)
{
}
