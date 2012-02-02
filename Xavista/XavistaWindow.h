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

#if !defined Xavista_WINDOW_H
#define Xavista_WINDOW_H

#include <QMainWindow>

#include "ui_XavistaWindow.h"

class XavistaWindow : public QMainWindow, public Ui::XavistaWindow
{
	Q_OBJECT
public:
	XavistaWindow(QWidget * parent=0);
	~XavistaWindow();
public slots:
	void calculate();
};

#endif // Xavista_WINDOW_H
