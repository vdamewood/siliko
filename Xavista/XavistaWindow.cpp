/*
 *  XavistaWindow.cpp: Main window.
 *  Copyright 2012 Vincent Damewood
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
	XaviNumber value = XaviParse(input->text().toUtf8().data());
	switch (value.status) {
	case S_INTEGER:
		output->setNum(value.i);
		break;
	case S_FLOAT:
		output->setNum(value.f);
		break;
	default:
		output->setText("An error occured.");
	}
}
