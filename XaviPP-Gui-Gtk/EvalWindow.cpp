#include <stdlib.h>
#include <string.h>

#include <gtkmm.h>

#include <XaviPP/CStringSource.hpp>
#include <XaviPP/InfixParser.hpp>
#include <XaviPP/Value.hpp>

#include "EvalWindow.hpp"

#if !defined SHARE_PATH
#error "You must define SHARE_PATH to build this file."
#endif

void EvalWindow::Calculate()
{
	Xavi::SyntaxTreeNode *ResultTree =
		Xavi::ParseInfix(
		new Xavi::CStringSource(
			MyInput->get_text().c_str()));
	Xavi::Value Value = ResultTree->Evaluate();
	delete ResultTree;

	char *ResultString = Value.ToCString();
	MyOutput->set_text(ResultString);
	delete[] ResultString;
}

EvalWindow::EvalWindow()
	:MyBuilder(0),
	MyWindow(0),
	MyButton(0),
	MyInput(0),
	MyOutput(0)
{
	MyBuilder = Gtk::Builder::create_from_file(SHARE_PATH "/EvalWindow.glade");
	MyBuilder->get_widget("EvalWindow", MyWindow);
	MyBuilder->get_widget("CalculateButton", MyButton);
	MyBuilder->get_widget("Input", MyInput);
	MyBuilder->get_widget("Output", MyOutput);

	MyButton->signal_clicked().connect(
		sigc::mem_fun(
		*this,
		&EvalWindow::Calculate));
	MyInput->signal_activate().connect(
		sigc::mem_fun(
		*this,
		&EvalWindow::Calculate));
	MyWindow->show();
}
