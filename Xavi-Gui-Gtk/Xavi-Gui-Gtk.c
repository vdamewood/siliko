#include <gtk/gtk.h>

#include <Xavi/FunctionCaller.h>

#include "EvalWindow.h"

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	XaviFunctionCallerSetUp();
	GtkBuilder *MainWindow = EvalWindowNew();
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(MainWindow, "EvalWindow")));
	gtk_main();
	XaviFunctionCallerTearDown();
	return 0;
}
