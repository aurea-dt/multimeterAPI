////////////////////////////////////////////////////////////////////////////////
/**
*	@file	DMM_MeterWidget.cpp
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	cpp template
*/
////////////////////////////////////////////////////////////////////////////////
#include "DMM_MeterWidget.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <cstring>
#include "drawTools.h"
#include <iomanip>
#include <gdk/gdk.h>

//------------------------------------------------------------------------------
DMM_MeterWidget::DMM_MeterWidget(GooCanvasItem* root)
{
	drag = false;
	reset();
	meterGroup = goo_canvas_group_new (root, NULL);
	goo_canvas_item_translate(meterGroup, 128,128);
	goo_canvas_item_add_child (root,meterGroup, 0);
	
	goo_canvas_path_new (meterGroup, getArcString(130, 20, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8, NULL);
	goo_canvas_path_new (meterGroup, getArcString(200, 20, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8, NULL);	
	goo_canvas_path_new (meterGroup, getArcString(150, 50, 2*M_PI*0.9999, 128, 128).c_str(), "fill-color-rgba", 0xA0A0A006, "stroke-color-rgba", 0xFEFEFEF8,NULL);

	meanValueArc = goo_canvas_path_new (meterGroup, getArcString(135, 80, 2*M_PI*0.001, 128, 128).c_str(), "fill-color-rgba", (hsvToRgb(2*M_PI*0.10, 80, 100)&0xFFFFFF00)|(int)(255*0.3), "stroke-color-rgba", 0, NULL);
	minValueArc = goo_canvas_path_new (meterGroup, getArcString(130, 20, 2*M_PI*0.001, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.1, 80, 100), "stroke-color-rgba", 0x0F0F0F0F, NULL);
	maxValueArc = goo_canvas_path_new (meterGroup, getArcString(200, 20, 2*M_PI*0.001, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.1, 80, 100), "stroke-color-rgba", 0x0F0F0F0F, NULL);	
	currentValueArc = goo_canvas_path_new (meterGroup, getArcString(150, 50, 2*M_PI*0.001, 128, 128).c_str(), "fill-color-rgba", hsvToRgb(2*M_PI*0.1, 80, 100), "stroke-color-rgba", 0x0F0F0F0F,NULL);

	currentValue =  goo_canvas_text_new(meterGroup, "0", 128, 128, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 70px", "fill-color-rgba", 0x505050FF, NULL);
	minValue =  goo_canvas_text_new(meterGroup, "0", 128, 178, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	maxValue =  goo_canvas_text_new(meterGroup, "0", 128, 78, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	
	typeLabel =  goo_canvas_text_new(meterGroup, "?", 128, 30, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	unitsLabel =  goo_canvas_text_new(meterGroup, "?", 128, 226, -1, GTK_ANCHOR_CENTER, "font", "Arial Bold 30px", "fill-color-rgba", 0x505050FF, NULL);
	
	g_signal_connect(meterGroup, "enter-notify-event", (GCallback)onMouseOver, this);
	g_signal_connect(meterGroup, "leave-notify-event", (GCallback)onMouseOut, this);
	g_signal_connect(meterGroup, "scroll-event", (GCallback)onScroll, this);
	g_signal_connect(meterGroup, "button-press-event", (GCallback)onMousDown, this);
	g_signal_connect(meterGroup, "button-release-event", (GCallback)onMouseUp, this);
	g_signal_connect(meterGroup, "motion-notify-event", (GCallback)onMouseMove, this);
	
}
//------------------------------------------------------------------------------
void DMM_MeterWidget::reset()
{
 	value = valueTemp = min = minTemp = max = maxTemp = mean = meanTemp = 0;
}
//------------------------------------------------------------------------------
void DMM_MeterWidget::update(float current_, float max_, float min_, float mean_, const char* units_, const char* type_, float range_)
{
	if(range!=range_)
		reset();
	value = current_;
	max = max_;
	min = min_;
	mean = mean_;
	range = range_;
	units = units_;
	type = type_;
	
	//draw(value, max, min, mean, units.c_str(), type.c_str(), range);
	g_timeout_add(2, animation, this);
}
//------------------------------------------------------------------------------
void DMM_MeterWidget::draw(float current_, float max_, float min_, float mean_, const char* units_, const char* type_, float range_)
{
	stringstream ss;
	string str;
	
	int ee;
	string formatedUnits;
	string formatedValueValue;
	
	if(current_ !=0 && strcmp(units_,"?")!=0)
	{
		formatedValueValue = formatValue(current_, units_, 1, ee, formatedUnits);
		g_object_set(currentValue, "text", formatedValueValue.c_str(), NULL);
		
		ss << setprecision(3) << max_*pow(1000, ee) << endl;
		getline(ss,str);	
		g_object_set(maxValue, "text", str.c_str(), NULL);

		ss << setprecision(3) << min_*pow(1000, ee) << endl;
		getline(ss,str);
		g_object_set(minValue, "text", str.c_str(), NULL);
	}
	else
	{
		formatedValueValue = formatValue(max_, units_, 1, ee, formatedUnits);
		g_object_set(maxValue, "text", formatedValueValue.c_str(), NULL);
			
		if(strcmp(units_,"?")==0)
		{
			g_object_set(currentValue, "text", "?", NULL);
		}
		else
		{
			ss << setprecision(3) << current_*pow(1000, ee) << endl;
			getline(ss,str);
			g_object_set(currentValue, "text", str.c_str(), NULL);
		}
	}
	
	g_object_set(typeLabel, "text", type_, NULL);
	g_object_set(unitsLabel, "text", formatedUnits.c_str(), NULL);		

	float maximum = range_;	
	g_object_set(currentValueArc, "data", getArcString(150, 50, 2*M_PI*current_/maximum+M_PI*0.001, 128, 128).c_str(), NULL);		
	g_object_set(maxValueArc, "data", getArcString(200, 20, 2*M_PI*max_/maximum, 128, 128).c_str(), NULL);
	g_object_set(minValueArc, "data", getArcString(130, 20, 2*M_PI*min_/maximum, 128, 128).c_str(), NULL);
	g_object_set(meanValueArc, "data", getArcString(135, 80, 2*M_PI*mean_/maximum, 128, 128).c_str(), NULL);
	
	g_object_set(currentValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*current_/maximum, 80, 100), NULL);
	g_object_set(minValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*min_/maximum, 80, 100), NULL);
	g_object_set(maxValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*max_/maximum, 80, 100), NULL);
	g_object_set(meanValueArc, "fill-color-rgba", hsvToRgb(2*M_PI*mean_/maximum, 80, 100)&0xFFFFFF00|(int)(255*0.3), NULL);
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::drawStep()
{
	float delta = range/128;
	if(minTemp!=min || maxTemp!=max|| valueTemp!=value || meanTemp!=mean)
	{
		if(valueTemp - value < 0)
			valueTemp = ((valueTemp+delta)>value-delta?value:valueTemp+delta);
		else
			valueTemp = ((valueTemp-delta)<value+delta?value:valueTemp-delta);
	
		if(minTemp - min < 0)
			minTemp = ((minTemp+delta)>min-delta?min:minTemp+delta);
		else
			minTemp = ((minTemp-delta)<min+delta?min:minTemp-delta);
			
		if(maxTemp - max < 0)
			maxTemp = ((maxTemp+delta)>max-delta?max:maxTemp+delta);
		else
			maxTemp = ((maxTemp-delta)<max+delta?max:maxTemp-delta);
			
		if(meanTemp - mean < 0)
			meanTemp = ((meanTemp+delta)>mean-delta?mean:meanTemp+delta);
		else
			meanTemp = ((meanTemp-delta)<mean+delta?mean:meanTemp-delta);
		draw(valueTemp, maxTemp, minTemp, meanTemp, units.c_str(), type.c_str(), range);	
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onMouseOver(GooCanvasItem *item, GooCanvasItem *target_item, GdkEventFocus *event, void* user_data)
{
	cout << "onMouseOver" << endl;
	gdk_window_set_cursor (event->window,  gdk_cursor_new_from_name(gdk_display_get_default(), "openhand"));
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onMouseOut(GooCanvasItem *item, GooCanvasItem *target_item, GdkEventFocus *event, void* user_data)
{
	cout << "onMouseOut" << endl;
	gdk_window_set_cursor (event->window,  gdk_cursor_new(GDK_LEFT_PTR));
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onMousDown(GooCanvasItem *item, GooCanvasItem *target_item, GdkEventButton *event, void* user_data)
{
	cout << "onMousDown" << endl;
	((DMM_MeterWidget*)user_data)->drag=true;
	((DMM_MeterWidget*)user_data)->xt=event->x;	
	((DMM_MeterWidget*)user_data)->yt=event->y;	
	
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onMouseUp(GooCanvasItem *item, GooCanvasItem *target_item,  GdkEventButton *event, void* user_data)
{
	cout << "onMouseUp" << endl;

	((DMM_MeterWidget*)user_data)->drag=false;
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onMouseMove(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventMotion *event, void* user_data)
{
	if(((DMM_MeterWidget*)user_data)->drag)
	{
		int newx = event->x-((DMM_MeterWidget*)user_data)->xt;	
		int newy = event->y-((DMM_MeterWidget*)user_data)->yt;	
		goo_canvas_item_translate (item, newx, newy);
	}
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::onScroll(GooCanvasItem *item, GooCanvasItem  *target_item, GdkEventScroll *event, void* user_data)
{
	cout << "on_scroll_event" << endl;
	switch(event->direction)
	{
		case GDK_SCROLL_UP:
			cout << "GDK_SCROLL_UP" << endl;
			goo_canvas_item_scale(item, 1.1,1.1);
			break;
		case GDK_SCROLL_DOWN:
			cout << "GDK_SCROLL_DOWN" << endl;
			goo_canvas_item_scale(item, 0.9,0.9);
			break;
		default:
			break;
	}
}
//------------------------------------------------------------------------------
int DMM_MeterWidget::animation(void* data)
{
	return ((DMM_MeterWidget*)data)->drawStep();
}
//------------------------------------------------------------------------------
 
