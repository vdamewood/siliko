#include <exception>
#include <iostream>

#include <gtkmm.h>

#include <XaviPP/FunctionCaller.hpp>

#include "EvalWindow.hpp"

int main(int argc, char *argv[])
try
{
	Gtk::Main XaviPPGuiGtk(argc, argv);
	Xavi::FunctionCaller::SetUp();

	EvalWindow Window;
	Gtk::Main::run(Window.Window());
	Xavi::FunctionCaller::TearDown();
	return 0;
}
catch(Gtk::BuilderError &e)
{
	std::cout << e.what() << std::endl;
}
/*catch(...)
{
	std::cout << "WTF?\n";
}*/
