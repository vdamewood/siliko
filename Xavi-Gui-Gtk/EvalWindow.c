#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <glade/glade.h>

#include <Xavi/StringSource.h>
#include <Xavi/InfixParser.h>

#if !defined SHARE_PATH
#error "You must define SHARE_PATH to build this file."
#endif

static char *ValueToString(XaviValue Value)
{
	int ValueSize = snprintf(NULL, 0, "%i", Value.Integer) + 1;
	char *ValueString = malloc(ValueSize);
	snprintf(ValueString, ValueSize, "%i", Value.Integer);
	return ValueString;
}

static void Calculate(GtkWidget *Widget, gpointer EvalWindow)
{
	XaviSyntaxTreeNode *ResultTree =
		XaviParseInfix(
		XaviStringSourceNew(
		gtk_entry_get_text(
		GTK_ENTRY(
		glade_xml_get_widget(GLADE_XML(EvalWindow), "Input")))));
	XaviValue Value = XaviSyntaxTreeEvaluate(ResultTree);
	XaviSyntaxTreeDelete(ResultTree);

	char *ResultString = ValueToString(Value);
	gtk_label_set_text(
		GTK_LABEL(
			glade_xml_get_widget(
			GLADE_XML(EvalWindow), "Output")),
		ResultString);
	free(ResultString);
}

GladeXML *EvalWindowNew(void)
{
	GladeXML *EvalWindow;
	EvalWindow = glade_xml_new(SHARE_PATH "/EvalWindow.glade", NULL, NULL);

	g_signal_connect(
		G_OBJECT(glade_xml_get_widget(EvalWindow, "EvalWindow")),
		"delete_event",
		G_CALLBACK(gtk_main_quit),
		NULL);
	g_signal_connect(
		G_OBJECT(glade_xml_get_widget(EvalWindow, "CalculateButton")),
		"clicked",
		G_CALLBACK(Calculate),
		EvalWindow);
	g_signal_connect(
		G_OBJECT(glade_xml_get_widget(EvalWindow, "Input")),
		"activate",
		G_CALLBACK(Calculate),
		EvalWindow);

	return EvalWindow;
}
