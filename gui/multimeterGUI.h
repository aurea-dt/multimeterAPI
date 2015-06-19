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
#include "drawTools.h"
#include "DMM_MeterWidget.h"
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
	
	DMM_MeterWidget* meter;
	
//	GooCanvasItem* meterGroup;
//	GooCanvasItem* currentValue;
//	GooCanvasItem* minValue;
//	GooCanvasItem* maxValue;
//	
//	GooCanvasItem* currentValueArc;
//	GooCanvasItem* minValueArc;
//	GooCanvasItem* maxValueArc;
//	GooCanvasItem* meanValueArc;
//	
//	GooCanvasItem* typeLabel;
//	GooCanvasItem* unitsLabel;
	
	void onRX();
	static int on_delete_event (GtkWidget *window, GdkEvent  *event, void* userdata);
	static int on_keyboard_event (GtkWidget *window, GdkEvent  *event, void* userdata);


 public:
 	MultimeterGUI(const char* portName, int speed, const char* settings);
};
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif
