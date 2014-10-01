/*
 * XaviTree.h: Functions to manipulate abstract syntax trees.
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

#if !defined Xavi_TREE_H
#define Xavi_TREE_H

#include <string>
#include <list>

#include "Value.hpp"

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
	class SyntaxTreeNode
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

	class FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(float NewValue);
		virtual Value GetValue(void);
		virtual void Negate(void);
	private:
		float Value;
	};

	class BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(void);
		BranchNode(std::string NewFunctionId);
		virtual ~BranchNode();
		virtual Value GetValue(void);
		virtual void Negate(void);

		void SetId(std::string NewId);
		void PushLeft(SyntaxTreeNode *);
		void PushRight(SyntaxTreeNode *);
		bool GraftLeft(SyntaxTreeNode *);
		bool GraftRight(SyntaxTreeNode *);
	private:
		bool IsNegated;
		std::string FunctionId;
		std::list<SyntaxTreeNode *> Children;
	};

	class SyntaxErrorNode : public SyntaxTreeNode
	{
	public:
		virtual ~SyntaxErrorNode(void);
		virtual Value GetValue(void);
		virtual void Negate(void);
	};
}

#endif // Xavi_TREE_H
