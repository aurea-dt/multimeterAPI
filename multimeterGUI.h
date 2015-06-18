////////////////////////////////////////////////////////////////////////////////
/**
*	@file	multimeterGUI.h
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	header template
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef MULTIMETERGUI_H
#define MULTIMETERGUI_H
//#include <clutter/clutter.h>
#include <goocanvas.h>
#include "ADT_Multimeter.h"
// your public header include
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// the declaration of your class...
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class MultimeterGUI: public ADT_Multimeter
{
 private:
	
	GtkWidget* window;
	GtkWidget* canvas;
	GooCanvasItem* root;
	GooCanvasItem* meterGroup;
	GooCanvasItem* currentValue;
	GooCanvasItem* minValue;
	GooCanvasItem* maxValue;
	
	GooCanvasItem* currentValueArc;
	GooCanvasItem* minValueArc;
	GooCanvasItem* maxValueArc;
	GooCanvasItem* meanValueArc;
	
	GooCanvasItem* typeLabel;
	GooCanvasItem* unitsLabel;
	
	void onRX();
	static gboolean on_delete_event (GtkWidget *window, GdkEvent  *event, void* userdata);
	static gboolean on_keyboard_event (GtkWidget *window, GdkEvent  *event, void* userdata);

	string formatValue(long double value, const char *units, int exp, int&ee, string &formatedUnits);
 public:
 	MultimeterGUI(const char* portName, int speed, const char* settings);
};
//------------------------------------------------------------------------------
//void createClutterArc(ClutterActor* path, float angle, float width, ClutterColor color, ClutterPoint center);
//------------------------------------------------------------------------------
int hsvToRgb(float h, float s, float v);
string getArcString(float radius, float awidth, float range, float x, float y);
#endif
