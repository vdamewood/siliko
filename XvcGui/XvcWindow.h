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
