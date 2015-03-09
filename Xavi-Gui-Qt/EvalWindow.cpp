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

#include <Xavi/FunctionCaller.h>
#include <Xavi/InfixParser.h>

#include "QStringSource.hpp"
#include "EvalWindow.hpp"

EvalWindow::EvalWindow(QWidget * parent)
	: QMainWindow(parent)
{
	XaviFunctionCallerSetUp();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

EvalWindow::~EvalWindow()
{
	XaviFunctionCallerTearDown();
}

void EvalWindow::calculate()
{
	XaviSyntaxTreeNode *MyTree = XaviParseInfix(
		QStringSourceNew(input->text()));

	XaviValue result = XaviSyntaxTreeEvaluate(MyTree);
	XaviSyntaxTreeDelete(MyTree);

	char *text = XaviValueToString(result);
	output->setText(text);
	std::free(text);
}
