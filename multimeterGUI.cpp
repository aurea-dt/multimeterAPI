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
#include <iomanip>
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
	
	meterGroup = goo_canvas_group_new (root, NULL);
	goo_canvas_item_translate(meterGroup, 128,128);
	goo_canvas_item_add_child (root,meterGroup, 0);
	
	goo_canvas_path_new (meterGroup, getArcString(130, 20, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8, NULL);
	goo_canvas_path_new (meterGroup, getArcString(200, 20, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8, NULL);	
	goo_canvas_path_new (meterGroup, getArcString(150, 50, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8,NULL);

	meanValueArc = goo_canvas_path_new (meterGroup, getArcString(135, 80, 2*M_PI*0.95, 128, 128).c_str(), "fill-color-rgba", (hsvToRgb(2*M_PI*0.95, 80, 100)&0xFFFFFF00)|(int)(255*0.3), "stroke-color-rgba", 0, NULL);
	minValueArc = goo_canvas_path_new (meterGroup, getArcString(130, 20, 2*M_PI*0.2, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.2, 80, 100), "stroke-color-rgba", 0x0F0F0F0F, NULL);
	maxValueArc = goo_canvas_path_new (meterGroup, getArcString(200, 20, 2*M_PI*0.9, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.9, 80, 100), "stroke-color-rgba", 0x0F0F0F0F, NULL);	
	currentValueArc = goo_canvas_path_new (meterGroup, getArcString(150, 50, 2*M_PI*0.4, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.4, 80, 100), "stroke-color-rgba", 0x0F0F0F0F,NULL);



	currentValue =  goo_canvas_text_new(meterGroup, "000.000", 128, 128, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 70px", "fill-color-rgba", 0x505050FF, NULL);
	minValue =  goo_canvas_text_new(meterGroup, "000.000", 128, 178, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	maxValue =  goo_canvas_text_new(meterGroup, "000.000", 128, 78, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	
	typeLabel =  goo_canvas_text_new(meterGroup, "VDC", 128, 30, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	unitsLabel =  goo_canvas_text_new(meterGroup, "mV", 128, 226, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	
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
	cout << "MultimeterGUI::onRX()" << endl;
	stringstream ss;
	string str;
	
	int ee;
	string formatedUnits;
	string formatedValueValue;
	
	if(getCurrent().value !=0 && getCurrent().units.compare("?")!=0)
	{
		formatedValueValue = formatValue(getCurrent().value, getCurrent().units.c_str(), 1, ee, formatedUnits);
		g_object_set(currentValue, "text", formatedValueValue.c_str(), NULL);
		
		ss << setprecision(3) << getMax()*pow(1000, ee) << endl;
		getline(ss,str);	
		g_object_set(maxValue, "text", str.c_str(), NULL);

		ss << setprecision(3) << getMin()*pow(1000, ee) << endl;
		getline(ss,str);
		g_object_set(minValue, "text", str.c_str(), NULL);
	}
	else
	{
		formatedValueValue = formatValue(getMax(), getCurrent().units.c_str(), 1, ee, formatedUnits);
		g_object_set(maxValue, "text", formatedValueValue.c_str(), NULL);
			
		if(getCurrent().units.compare("?")==0)
		{
			g_object_set(currentValue, "text", "?", NULL);
		}
		else
		{
			ss << setprecision(3) << getCurrent().value*pow(1000, ee) << endl;
			getline(ss,str);
			g_object_set(currentValue, "text", str.c_str(), NULL);
		}
	}
	
	g_object_set(typeLabel, "text", getCurrent().type.c_str(), NULL);
	g_object_set(unitsLabel, "text", formatedUnits.c_str(), NULL);		

	float maximum = range;	
	g_object_set(currentValueArc, "data", getArcString(150, 50, 2*M_PI*getCurrent().value/maximum+M_PI*0.001, 128, 128).c_str(), NULL);		
	g_object_set(maxValueArc, "data", getArcString(200, 25, 2*M_PI*getMax()/maximum, 128, 128).c_str(), NULL);
	g_object_set(minValueArc, "data", getArcString(125, 25, 2*M_PI*getMin()/maximum, 128, 128).c_str(), NULL);
	g_object_set(meanValueArc, "data", getArcString(135, 80, 2*M_PI*getMean()/maximum, 128, 128).c_str(), NULL);
	
	g_object_set(currentValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*getCurrent().value/maximum, 80, 100), NULL);
	g_object_set(minValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*getMin()/maximum, 80, 100), NULL);
	g_object_set(maxValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*getMax()/maximum, 80, 100), NULL);
	g_object_set(meanValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*getMean()/maximum, 80, 100)&0xFFFFFF00|(int)(255*0.3), NULL);
}
//------------------------------------------------------------------------------
string MultimeterGUI::formatValue(long double value, const char *units, int exp, int &ee, string &formatedUnits)
{
	if(exp <0 || exp > 3)
		return "?";

	int uni=8;
	const char* prefix[17] = {"Y", "Z", "E", "P", "T", "G", "M", "k", "", "m", "μ", "n", "p", "f", "a", "z", "y"};
	const char* expSym[3] = {"", "²", "³"};
	if (value!=0)
	{
		if (value <1)
			do
			{
				value *= pow(1000, exp);
				uni++;

			}while (value <1);// || numero >= 1000);

		if (value >= 1000)
			do
			{
				value /= pow(1000, exp);
				uni--;
			}while (value >= 1000);//(numero <1 || numero >= 1000);

	}
	ee=uni-8;
	stringstream ss;
	string formatedValue;
	ss  << setprecision(3) << value << endl;
	getline (ss, formatedValue);
		
	ss << prefix[uni] << units << expSym[exp-1] << endl;
	getline (ss, formatedUnits);
	
	return formatedValue;
}
//------------------------------------------------------------------------------
string getArcString(float radius, float awidth, float range, float x, float y)
{

	if(range==2*M_PI)
		range=range*0.9999;
	float myAngle = M_PI/2;
	float x1  = x + radius * cos(myAngle);
	float y1  = y + radius * sin(myAngle);
	float x2  = x + radius * cos(myAngle+range);
	float y2  = y + radius * sin(myAngle+range);
	float x3  = x + (radius+awidth) * cos(myAngle+range);
	float y3  = y + (radius+awidth)* sin(myAngle+range);
	float x4  = x + (radius+awidth) * cos(myAngle);
	float y4  = y + (radius+awidth)* sin(myAngle);
	float r2= radius+awidth;
	bool arc_flag = (range>M_PI || range<-1*M_PI);
	bool sweep_flag=!(range<0);
	stringstream ss;
	string str;
	ss << "M" << x1 << "," << y1 << " A" << radius << "," << radius << " 0 " << arc_flag << " " << sweep_flag << " " << x2 << "," << y2 << " L" << x3 << " " << y3 << " A" << r2 << "," << r2 << " 0 " << arc_flag << " " << !sweep_flag << " " << x4 << "," << y4 << " Z" << endl;
	getline(ss,str);
	return str;	
}
//------------------------------------------------------------------------------
int hsvToRgb(float h, float s, float v) 
{
	float r, g, b;
	int i;
	float f, p, q, t;
	//h=h+M_PI*0.2;

	// Make sure our arguments stay in-range
	h = max((float)-360, min((float)360.0, h));
	s = max((float)0, min((float)100.0, s));
	v = max((float)0, min((float)100.0, v));
	
	h=h/M_PI*180;
	h=(-1*h+360)/2;
	// We accept saturation and value arguments from 0 to 100 because that's
	// how Photoshop represents those values. Internally, however, the
	// saturation and value are calculated from a range of 0 to 1. We make
	// That conversion here.
	s /= 100.0;
	v /= 100.0;
	
	if(s == 0) 
	{
		// Achromatic (grey)
		r = g = b = v;
		return floor(r * 255)*0xFF000000 + floor(g * 255)*0xFF0000 + floor(b * 255)*0xFF00+0.9*0xFF;
	}
	
	h /= 60; // sector 0 to 5
	i = floor(h);
	f = h - i; // factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch(i) 
	{
		case 0:
			r = v;
			g = t;
			b = p;
			break;
			
		case 1:
			r = q;
			g = v;
			b = p;
			break;
			
		case 2:
			r = p;
			g = v;
			b = t;
			break;
			
		case 3:
			r = p;
			g = q;
			b = v;
			break;
			
		case 4:
			r = t;
			g = p;
			b = v;
			break;
			
		default: // case 5:
			r = v;
			g = p;
			b = q;
	}
	int rr = r * 255;
	int gg = g * 255;
	int bb = b * 255;
	int aa = 0.9*255;
	int color =  rr<<24 | gg<<16 | bb<<8 | aa;
	return color;

}
//------------------------------------------------------------------------------

