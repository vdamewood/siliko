/*
 * SyntaxTree.hpp: Abstract syntax tree classes.
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

#if !defined XAVI_SYNTAX_TREE_H
#define XAVI_SYNTAX_TREE_H

#include <list>

#include "W32Dll.hpp"
#include "Value.hpp"

namespace Xavi
{
	class API SyntaxTreeNode
	{
	public:
		virtual ~SyntaxTreeNode(void) { };
		virtual Value GetValue(void) = 0;
		virtual void Negate(void) = 0;
	};

	class IntegerNode : public SyntaxTreeNode
	{
	public:
		IntegerNode(int NewValue);
		virtual Value GetValue(void);
		virtual void Negate(void);
	private:
		int Value;
	};

	class API FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(float NewValue);
		virtual Value GetValue(void);
		virtual void Negate(void);
	private:
		float Value;
	};

	class API BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(void);
		BranchNode(const char *NewFunctionId);
		virtual ~BranchNode();
		virtual Value GetValue(void);
		virtual void Negate(void);

		void SetId(const char *NewId);
		void PushLeft(SyntaxTreeNode *);
		void PushRight(SyntaxTreeNode *);
		bool GraftLeft(SyntaxTreeNode *);
		bool GraftRight(SyntaxTreeNode *);
	private:
		bool IsNegated;
		char *FunctionId;
		std::list<SyntaxTreeNode *> Children;
	};

	class API SyntaxErrorNode : public SyntaxTreeNode
	{
	public:
		virtual ~SyntaxErrorNode(void);
		virtual Value GetValue(void);
		virtual void Negate(void);
	};
}

#endif // XAVI_SYNTAX_TREE_H
