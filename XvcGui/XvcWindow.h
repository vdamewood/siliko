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

#if !defined XVC_WINDOW_H
#define XVC_WINDOW_H

#include <QMainWindow>

#include "ui_XvcWindow.h"

class XvcWindow : public QMainWindow, public Ui::XvcWindow
{
	Q_OBJECT
public:
	XvcWindow(QWidget * parent=0);
	~XvcWindow();
public slots:
	void calculate();
};

#endif // XVC_WINDOW_H
