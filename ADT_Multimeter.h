////////////////////////////////////////////////////////////////////////////////
/**
*	@file	ADT_Multimeter.h
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	header template
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef ADT_MULTIMETER_H
#define ADT_MULTIMETER_H

// your public header include
//------------------------------------------------------------------------------
#include "ADT_SerialPort.h"
#include <ctime>
#include <vector>
//------------------------------------------------------------------------------

// the declaration of your class...
typedef struct
{
	float value;
	string type;
	string units;
	time_t time;
}DMMmesurement;
//------------------------------------------------------------------------------
typedef struct
{
	float min;
	float max;
	string units;
	float factor;
}DMMrange;
//------------------------------------------------------------------------------
typedef struct
{
	string type;
	string units;
	string text;
	vector <DMMrange> rangeList;
}DMMfunction;
//------------------------------------------------------------------------------
typedef struct
{
	string dataFormat;
	vector <DMMfunction> functionList;
}DMMcfg;
//------------------------------------------------------------------------------
class ADT_Multimeter: public ADT_SerialPort
{
 private:
 	int timerSource;
 	DMMcfg configuration;
 	
 	vector <DMMmesurement> measurements;
	ofstream* recordsFile;

 	static int rxTimer_cb(void* data);
 	int record(DMMmesurement measurement);
 	void loadCFG(const char* filename);	
	void onGetData();
	virtual void onRX();
	
 public:
 	float range;
  	int newFile();
 	float getMin() const;
 	float getMax() const;
 	float getMean() const;
 	DMMmesurement getMeasurement(float value, const char* type, const char*units) const;
 	float findRange(DMMmesurement measurement) const;
	DMMmesurement getCurrent() const;
 	int setTimer(int mseconds);
	ADT_Multimeter(const char* portName, int speed, const char* settings);
};
//------------------------------------------------------------------------------
#endif
