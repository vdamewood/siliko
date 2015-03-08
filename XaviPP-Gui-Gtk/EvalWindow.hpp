#include <gtkmm.h>

class EvalWindow
{
public:
	EvalWindow();
	void Calculate();
	Gtk::Window &Window() { return *MyWindow; }
private:
	Glib::RefPtr<Gtk::Builder> MyBuilder;
	Gtk::Window *MyWindow;
	Gtk::Button *MyButton;
	Gtk::Entry *MyInput;
	Gtk::Label *MyOutput;
};
