/* Main.c: Entry point for GUI program
 * Copyright 2012-2021 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>

#include <Siliko/FunctionCaller.h>

#include "EvalWindow.h"

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	SilikoFunctionCallerSetUp();
	GtkBuilder *MainWindow = EvalWindowNew();
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(MainWindow, "EvalWindow")));
	gtk_main();
	SilikoFunctionCallerTearDown();
	return 0;
}
