////////////////////////////////////////////////////////////////////////////////
/**
*	@file	ADT_Multimeter.cpp
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	cpp template
*/
////////////////////////////////////////////////////////////////////////////////
#include "ADT_Multimeter.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
using namespace std;
//------------------------------------------------------------------------------
ADT_Multimeter::ADT_Multimeter(const char* portName, int speed, const char* settings):ADT_SerialPort(portName, speed, settings)
{
	cout << "ADT_Multimeter" << endl;
	timerSource = g_timeout_add(2000, rxTimer_cb, this);
	recordsFile = new ofstream();
	
	loadCFG("protek506.xml");
}
//------------------------------------------------------------------------------
int ADT_Multimeter::rxTimer_cb(void* data)
{
	((ADT_Multimeter*)data)->sendData("OK", 2);
	return true;
}
//------------------------------------------------------------------------------
void ADT_Multimeter::loadCFG(const char* filename)
{
    
     /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION
    
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    
    doc = xmlReadFile(filename, NULL, 0);
    if (doc != NULL)
    {
        root_element = xmlDocGetRootElement(doc);
        xmlNode *cur_node = NULL;
    	for (cur_node = root_element; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			configuration.dataFormat=string((const char*)xmlGetProp(cur_node, (const xmlChar*)"dataFormat"));
			xmlNode *func_node = NULL;
			for (func_node = cur_node->children; func_node; func_node = func_node->next)
			{
				if (func_node->type == XML_ELEMENT_NODE)
				{
					DMMfunction function;
					function.type=string((const char*)xmlGetProp(func_node, (const xmlChar*)"type"));
					function.units=string((const char*)xmlGetProp(func_node, (const xmlChar*)"units"));
					function.text=string((const char*)xmlGetProp(func_node, (const xmlChar*)"text"));
					
					xmlNode *range_node = NULL;
					for (range_node = func_node->children; range_node; range_node = range_node->next)
					{
						if (range_node->type == XML_ELEMENT_NODE)
						{
							DMMrange range;
							range.min = atof((const char*)xmlGetProp(range_node, (const xmlChar*)"min"));
							range.max = atof((const char*)xmlGetProp(range_node, (const xmlChar*)"max"));
							range.units = string((const char*)xmlGetProp(range_node, (const xmlChar*)"units"));
							range.factor = atof((const char*)xmlGetProp(range_node, (const xmlChar*)"factor"));
							function.rangeList.push_back(range);
						}
					}
					configuration.functionList.push_back(function);
				}
			}
	
		}
		
	}
    }
    else
    {
        cerr << "error: could not parse file " << filename << endl;
    }
}
//------------------------------------------------------------------------------
void ADT_Multimeter::onGetData()
{
	//cout << "ADT_Multimeter::onGetData()" << endl;
	cout << buffer << dec;
	stringstream ss;
	
	float value;
	string type, units;
	ss << buffer << endl;
	ss >> type >> value >> 	units;

	//cout << type << ", "<< value << ", " << units << endl;
	
	DMMmesurement measurement = getMeasurement(value, type.c_str(), units.c_str());
	record(measurement);
	range=findRange(measurement);
	onRX();
}
//------------------------------------------------------------------------------
int ADT_Multimeter::newFile()
{
	measurements.clear();
	if(recordsFile->is_open())
	{
		recordsFile->close();
	}
	time_t rawtime;
  	struct tm * timeinfo;
  	char buffer [20];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,20,"%F_%T.csv",timeinfo);
	puts (buffer);
	recordsFile->open (buffer);
	return true;
}
//------------------------------------------------------------------------------
int ADT_Multimeter::record(DMMmesurement measurement)
{
	if(measurements.size()==0 || strcmp(measurements[measurements.size()-1].type.c_str(), measurement.type.c_str())!=0 )
		newFile();
	measurements.push_back(measurement);
	//cout << measurement.type << ", " << measurement.value << ", " << measurement.units << ", " << measurement.time << endl;
	*recordsFile << measurement.type << ", " << measurement.value << ", " << measurement.units << ", " << measurement.time << endl;
	return measurements.size();	
}
//------------------------------------------------------------------------------
float ADT_Multimeter::getMin() const
{
	float min = measurements[0].value;
	for(unsigned int i=0; i<measurements.size(); i++)
	{
		if(measurements[i].value<min)
			min = measurements[i].value;
	}
	return min;
}
//------------------------------------------------------------------------------
float ADT_Multimeter::getMax() const
{
	float max = measurements[0].value;
	for(unsigned int i=0; i<measurements.size(); i++)
	{
		if(measurements[i].value>max)
			max = measurements[i].value;
	}
	return max;
}
//------------------------------------------------------------------------------
float ADT_Multimeter::getMean() const
{
	float mean =  0;
	for(unsigned int i=0; i<measurements.size(); i++)
	{
		mean += measurements[i].value;
	}
	return mean/measurements.size();
}
//------------------------------------------------------------------------------
DMMmesurement ADT_Multimeter::getMeasurement(float value, const char* type, const char*units) const
{
	float factor;
	DMMmesurement measurement;
	for(unsigned int f=0; f<configuration.functionList.size(); f++)
	{

		if(strcmp(type, configuration.functionList[f].type.c_str())==0)
		{
		//cout << "types: " << type << ", " << configuration.functionList[f].type << endl;
			for(unsigned int r=0; r<configuration.functionList[f].rangeList.size(); r++)
			{
				if(strcmp(units, configuration.functionList[f].rangeList[r].units.c_str())==0)
				{
					//cout << "units: " << units << ", " << configuration.functionList[f].rangeList[r].units.c_str() << endl;
					factor=configuration.functionList[f].rangeList[r].factor;

					measurement.value = value * factor;
					measurement.type = configuration.functionList[f].type;
					measurement.units = configuration.functionList[f].units;
					time(&measurement.time);
					return measurement;
				}
			}	
		}
	}
	measurement.value = 0;
	measurement.type = type;
	measurement.units = "?";
	time(&measurement.time);
	return measurement;
}
//------------------------------------------------------------------------------
 float ADT_Multimeter::findRange(DMMmesurement measurement) const
 {
	for(unsigned int f=0; f<configuration.functionList.size(); f++)
	{
		if( measurement.type.compare(configuration.functionList[f].type)==0 && measurement.units.compare(configuration.functionList[f].units)==0)
		{
			for(unsigned int r=0; r<configuration.functionList[f].rangeList.size(); r++)
			{
				float min = configuration.functionList[f].rangeList[r].min;
				float max = configuration.functionList[f].rangeList[r].max;
				float factor = configuration.functionList[f].rangeList[r].factor;
				float value = abs(getMax());
				if(value >= min*factor &&  value < max*factor)
				{
					return configuration.functionList[f].rangeList[r].max*configuration.functionList[f].rangeList[r].factor;
				}
			}	
		}
	}
	return range;
 }
//------------------------------------------------------------------------------
DMMmesurement ADT_Multimeter::getCurrent() const
{
	return measurements[measurements.size()-1];
}
//------------------------------------------------------------------------------
int ADT_Multimeter::setTimer(int mseconds)
{
	g_source_destroy (g_main_context_find_source_by_id(NULL, timerSource));
	timerSource = g_timeout_add(mseconds, rxTimer_cb, this);
	return timerSource;
}
//------------------------------------------------------------------------------
void ADT_Multimeter::onRX()
{
	cout << "ADT_Multimeter::onRX()" << endl;
}
//------------------------------------------------------------------------------
	
