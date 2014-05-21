/*
 *  XavistaWindow.cpp: Main window.
 *  Copyright 2012, 2014 Vincent Damewood
 *
 *  This file is part of Xavista.
 *
 *  Xavista is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Xavista is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Xavista. If not, see <http://www.gnu.org/licenses/>.
 */
#include "XavistaWindow.h"
#include "Xavi.h"

XavistaWindow::XavistaWindow(QWidget * parent)
	: QMainWindow(parent)
{
	XaviOpen();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

XavistaWindow::~XavistaWindow()
{
	XaviClose();
}

void XavistaWindow::calculate()
{
	XaviResult value = XaviEvaluate(input->text().toUtf8().data());
	switch (value.status) {
	case XAVI_RS_INTEGER:
		output->setNum(value.i);
		break;
	case XAVI_RS_FLOAT:
		output->setNum(value.f);
		break;
	case XAVI_RE_INTERNAL:
		output->setText("Internal error.");
		break;
	case XAVI_RE_MEMORY:
		output->setText("Out of memory.");
		break;
	case XAVI_RE_SYNTAX:
		output->setText("Syntax error.");
		break;
	case XAVI_RE_ZERO_DIV:
		output->setText("Division by zero error.");
		break;
	case XAVI_RE_FUNCTION:
		output->setText("Function not found.");
		break;
	case XAVI_RE_ARGUMENTS:
		output->setText("Bad argument count.");
		break;
	case XAVI_RE_DOMAIN:
		output->setText("Domain error.");
		break;
	case XAVI_RE_RANGE:
		output->setText("Range error.");
		break;
	default:
		output->setText("An error occured.");
	}
}
