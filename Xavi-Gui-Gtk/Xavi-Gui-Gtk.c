#include <gtk/gtk.h>
#include <glade/glade.h>

#include <Xavi/FunctionCaller.h>

#include "EvalWindow.h"

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	XaviFunctionCallerSetUp();
	GladeXML *MainWindow = EvalWindowNew();
	gtk_main();
	XaviFunctionCallerTearDown();
	return 0;
}
