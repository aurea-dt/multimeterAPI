////////////////////////////////////////////////////////////////////////////////
/**
*	@file	multimeterGUI.cpp
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	cpp template
*/
////////////////////////////////////////////////////////////////////////////////
#include "multimeterGUI.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
using namespace std;
//------------------------------------------------------------------------------
MultimeterGUI::MultimeterGUI(const char* portName, int speed, const char* settings):ADT_Multimeter(portName, speed, settings)
{
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 512, 512);
	gtk_widget_show (window);
	g_signal_connect (window, "delete_event", (GtkSignalFunc) on_delete_event, NULL);
	g_signal_connect (window, "key-press-event", (GtkSignalFunc) on_keyboard_event, this);
	gtk_window_set_title ((GtkWindow*)window, "DMM aurea-dt.com");
	canvas = goo_canvas_new ();
	gtk_widget_set_size_request (canvas, 512, 512);
	gtk_container_add (GTK_CONTAINER (window), canvas);
	gtk_widget_show (canvas);

	root = goo_canvas_get_root_item (GOO_CANVAS (canvas));
	
	meter = new DMM_MeterWidget(root);

}
//------------------------------------------------------------------------------
gboolean MultimeterGUI::on_delete_event(GtkWidget *window, GdkEvent *event, void* userdata)
{
	cout << "MultimeterGUI::on_delete_event" << endl;
	exit(0);
}
//------------------------------------------------------------------------------
gboolean MultimeterGUI::on_keyboard_event (GtkWidget *window, GdkEvent *event, void* userdata)
{
	cout << (char)((GdkEventKey*)event)->keyval << endl;
	switch(((GdkEventKey*)event)->keyval)
	{
		case 's':
			((MultimeterGUI*)userdata)->newFile();
			break;
		case 'f':
			gtk_window_fullscreen ((GtkWindow*)window);
			break;
		case 'u':
			gtk_window_unfullscreen ((GtkWindow*)window);
			break;
		default:
			break;
	}
	return false;
}
//------------------------------------------------------------------------------
void MultimeterGUI::onRX()
{
	//cout << "MultimeterGUI::onRX()" << endl;
	stringstream ss;
	string str;
	
	int ee;
	string formatedUnits;
	string formatedValueValue;
	
	meter->update(getCurrent().value, getMax(), getMin(), getMean(), getCurrent().units.c_str(), getCurrent().type.c_str(), range);
}
//------------------------------------------------------------------------------

