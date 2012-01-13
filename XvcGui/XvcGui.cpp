#include <QApplication>

#include "XvcWindow.h"

int main(int argc, char *argv[])
{
	QApplication XvcGui(argc, argv);
	XvcWindow * MainWindow = new XvcWindow;
	MainWindow->show();
	return XvcGui.exec();
}
