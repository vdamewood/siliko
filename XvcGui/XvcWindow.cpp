#include <iostream>

#include "XvcWindow.h"
#include "XVCalc.h"

XvcWindow::XvcWindow(QWidget * parent)
	: QMainWindow(parent)
{
	XvcOpen();
	setupUi(this);
	output->setText(QString("Constructor Called!"));
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
