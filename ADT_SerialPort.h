////////////////////////////////////////////////////////////////////////////////
// ADT_SerialPort.h
// Mario Chirinos Colunga
// Aurea Desarrollo Tecnológico.
// 2012/03/12
//------------------------------------------------------------------------------
// Notes:
//	c++ header tamplate
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ADT_SERIALPORT_H
#define ADT_SERIALPORT_H

// your public header include
//------------------------------------------------------------------------------
#include <glib.h>
#include <sys/stat.h>
#include <fcntl.h> // File control definitions
#include <unistd.h>
#include <termios.h> // POSIX terminal control definitionss
#include <string>
using namespace std;
//------------------------------------------------------------------------------

#define	BUFFERSIZE	255
//------------------------------------------------------------------------------
class ADT_SerialPort
{
 private:
 	const char* device;
	int fd;
	GIOChannel* channel;
	static int ttycallback(GIOChannel *source, GIOCondition condition, void *data);
	int sendData(int fd, const unsigned char *data,  int length) const;
	virtual void onGetData();
 protected:
  	unsigned char* buffer;
  	unsigned int bufferLength;

 public:
	int config(int speed, const char* settings);
 	int sendData(const unsigned char* data, unsigned int length) const;
 	int sendData(const char* data, unsigned int length) const;
 	int sendData(string data) const;
 	ADT_SerialPort(const char* device, int speed, const char* settings);
 	~ADT_SerialPort();
 	
};
//------------------------------------------------------------------------------
#endif
