////////////////////////////////////////////////////////////////////////////////
/**
*	@file	drawTools.h
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	header template
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H

// your public header include
//------------------------------------------------------------------------------
#include <string>
using namespace std;
//------------------------------------------------------------------------------

// the declaration of your class...
//------------------------------------------------------------------------------
int hsvToRgb(float h, float s, float v);
string getArcString(float radius, float awidth, float range, float x, float y);
string formatValue(long double value, const char *units, int exp, int &ee, string &formatedUnits);
//------------------------------------------------------------------------------
#endif
