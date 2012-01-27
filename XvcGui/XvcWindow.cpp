/*
 *  XvcWindow.cpp: Main window.
 *  Copyright 2012 Vincent Damewood
 *
 *  This file is part of XvcGui.
 *
 *  XvcGui is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  XvcGui is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XvcGui. If not, see <http://www.gnu.org/licenses/>.
 */
#include "XvcWindow.h"
#include "XVCalc.h"

XvcWindow::XvcWindow(QWidget * parent)
	: QMainWindow(parent)
{
	XvcOpen();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

XvcWindow::~XvcWindow()
{
	XvcClose();
}

void XvcWindow::calculate()
{	
	XvcStatus status = XvcParse(input->text().toUtf8().data());
	switch (status) {
	case S_INTEGER:
		output->setNum(XvcGetInteger());
		break;
	case S_FLOAT:
		output->setNum(XvcGetFloat());
		break;
	default:
		output->setText("An error occured.");
	}
}
