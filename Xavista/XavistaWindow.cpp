/*
 * XavistaWindow.cpp: Main window.
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

#include <Xavi++/FunctionCaller.hpp>
#include <Xavi++/InfixParser.hpp>
#include <Xavi++/CStringSource.hpp>

#include "QStringSource.hpp"
#include "XavistaWindow.hpp"

XavistaWindow::XavistaWindow(QWidget * parent)
	: QMainWindow(parent)
{
	Xavi::FunctionCaller::Initialize();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

XavistaWindow::~XavistaWindow()
{
	Xavi::FunctionCaller::Destroy();
}

void XavistaWindow::calculate()
{
	Xavi::InfixParser MyParser(new Xavi::Lexer(new QStringSource(input->text())));

	MyParser.Parse();
	Xavi::Value result = MyParser.SyntaxTree().GetValue();
	switch (result.Status())
	{
		case Xavi::Value::INTEGER:
			output->setNum(result.IntegerValue());
			break;
		case Xavi::Value::FLOAT:
			output->setNum(result.FloatValue());
			break;
		case Xavi::Value::MEMORY_ERR:
			output->setText("Out of memory.\n");
			break;
		case Xavi::Value::SYNTAX_ERR:
			output->setText("Syntax error.\n");
			break;
		case Xavi::Value::ZERO_DIV_ERR:
			output->setText("Division by zero error.\n");
			break;
		case Xavi::Value::BAD_FUNCTION:
			output->setText("Function not found.\n");
			break;
		case Xavi::Value::BAD_ARGUMENTS:
			output->setText("Bad argument count.\n");
			break;
		case Xavi::Value::DOMAIN_ERR:
			output->setText("Domain error.\n");
			break;
		case Xavi::Value::RANGE_ERR:
			output->setText("Range error.\n");
			break;
	}
}
