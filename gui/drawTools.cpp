////////////////////////////////////////////////////////////////////////////////
/**
*	@file	drawTools.cpp
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	cpp template
*/
////////////////////////////////////////////////////////////////////////////////
#include "drawTools.h"
#include <cmath>
#include <sstream>
#include <iomanip>
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
string formatValue(long double value, const char *units, int exp, int &ee, string &formatedUnits)
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

