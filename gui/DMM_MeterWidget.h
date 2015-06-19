////////////////////////////////////////////////////////////////////////////////
/**
*	@file	DMM_MeterWidget.h
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	header template
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef DMM_METERWIDGET_H
#define DMM_METERWIDGET_H

// your public header include
//------------------------------------------------------------------------------
#include <goocanvas.h>
#include <string>
using namespace std;
//------------------------------------------------------------------------------

// the declaration of your class...
//------------------------------------------------------------------------------
class DMM_MeterWidget
{
 private:
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
	
	bool drag;
	int xt;
	int yt;
	
	float value;
	float max;
	float min;
	float mean;
	float range;
	string units;
	string type;
	
	float valueTemp;
	float maxTemp;
	float minTemp;
	float meanTemp;
	
	static int onMouseOver(GooCanvasItem *item, GooCanvasItem *target_item, GdkEventFocus *event, void* user_data);
	static int onMouseOut(GooCanvasItem *item, GooCanvasItem *target_item, GdkEventFocus *event, void* user_data);
	static int onScroll(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventScroll *event, void* user_data);
	static int onMousDown(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventButton *event, void* user_data);
	static int onMouseUp(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventButton *event, void* user_data);
	static int onMouseMove(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventMotion *event, void* user_data);
			
 	static int animation(void* data);
 	void draw(float current, float max, float min, float mean, const char* units, const char* type, float range);
 	int drawStep();
 public:
 	void reset();
	void update(float current, float max, float min, float mean, const char* units, const char* type, float range);
 	DMM_MeterWidget(GooCanvasItem* root);
};
//------------------------------------------------------------------------------
#endif
