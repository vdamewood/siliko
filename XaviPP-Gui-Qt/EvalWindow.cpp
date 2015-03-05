/*
 * EvalWindow.cpp: Expression evaluation window
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

#include <XaviPP/FunctionCaller.hpp>
#include <XaviPP/InfixParser.hpp>

#include "QStringSource.hpp"
#include "EvalWindow.hpp"

EvalWindow::EvalWindow(QWidget * parent)
	: QMainWindow(parent)
{
	Xavi::FunctionCaller::SetUp();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

EvalWindow::~EvalWindow()
{
	Xavi::FunctionCaller::TearDown();
}

void EvalWindow::calculate()
{
	Xavi::SyntaxTreeNode *MyTree = Xavi::ParseInfix(
		new QStringSource(input->text()));

	Xavi::Value result = MyTree->Evaluate();

	char *text = result.ToCString();
	output->setText(text);
	delete[] text;
}
