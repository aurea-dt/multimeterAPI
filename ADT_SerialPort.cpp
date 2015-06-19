////////////////////////////////////////////////////////////////////////////////
// ADT_SerialPort.cpp
// Mario Chirinos Colunga
// Aurea Desarrollo Tecnológico.
// 2015/16/06
//------------------------------------------------------------------------------
// Notes:
//	c++ unit tamplate
//
////////////////////////////////////////////////////////////////////////////////
#include "ADT_SerialPort.h"

#include <iostream>
#include <cstdlib>
using namespace std;
//------------------------------------------------------------------------------
ADT_SerialPort::ADT_SerialPort(const char* _device, int speed, const char* settings)
{
	buffer = new unsigned char [BUFFERSIZE];
	device = _device;
	if ((fd = open(device, O_RDWR | O_NONBLOCK | O_NOCTTY ) ) < 0)
	{
		cerr << "could not open: " << device << endl;
		exit(1);
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
		channel = g_io_channel_unix_new (fd);
		g_io_add_watch(channel, G_IO_IN, &ttycallback, this);
		config(speed, settings);
		cout << "port " << device << " is now open" << endl;
	}
} 	
//------------------------------------------------------------------------------
ADT_SerialPort::~ADT_SerialPort()
{
	close(fd);
	delete[] buffer;
}
//------------------------------------------------------------------------------
int ADT_SerialPort::config(int speed, const char* settings)
{
	struct termios port_settings;      // structure to store the port settings in

   	tcgetattr(fd, &port_settings);

	speed_t portSpeed = B9600;
	
	switch(speed)
	{
		case 0:
			portSpeed=B0;
			break;
		case 50:
			portSpeed=B50;
			break;
		case 75:
			portSpeed=B75;
			break;
		case 110:
			portSpeed=B110;
			break;
		case 134:
			portSpeed=B134;
			break;
		case 150:
			portSpeed=B150;
			break;
		case 200:
			portSpeed=B200;
			break;
		case 300:
			portSpeed=B300;
			break;
		case 600:
			portSpeed=B600;
			break;
		case 1200:
			portSpeed=B1200;
			break;
		case 1800:
			portSpeed=B1800;
			break;
		case 2400:
			portSpeed=B2400;
			break;
		case 4800:
			portSpeed=B4800;
			break;
		case 9600:
			portSpeed=B9600;
			break;
		case 19200:
			portSpeed=B19200;
			break;
		case 38400:
			portSpeed=B38400;
			break;	
		default:
			portSpeed=B0;
			break;
	}

	cfsetispeed(&port_settings, portSpeed);    // set baud rates
	cfsetospeed(&port_settings, portSpeed);
	
	port_settings.c_cflag &= ~CSIZE;	/* Clear current char size mask */
	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_lflag &= ~ECHO;
	port_settings.c_cflag |= CRTSCTS;
		
	switch(settings[0])
	{
		case '5':
			port_settings.c_cflag |= CS5;    /* Select 5 data bits */
			break;
		case '6':
			port_settings.c_cflag |= CS6;    /* Select 6 data bits */
			break;
		case '7':
			port_settings.c_cflag |= CS7;    /* Select 7 data bits */
			break;
		case '8':
		default:
			port_settings.c_cflag |= CS8;    /* Select 8 data bits */
			break;
	}
	switch(settings[1])
	{
		case 'E':
			port_settings.c_cflag |= PARENB;    /* Enable parity */
			break;
		case 'N':
		default:
			port_settings.c_cflag &= ~PARENB;    /* Diable parity */
			break;
	}
	switch(settings[2])
	{
		case '1':
			port_settings.c_cflag &= ~CSTOPB;    /* Set one stop bit */
			break;
		case '2':
		default:
			port_settings.c_cflag |= CSTOPB;    /* Set two stop bits */
			break;
	}
//	tcflush(fd, TCIFLUSH);

	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
	return(fd);
}
//------------------------------------------------------------------------------
int ADT_SerialPort::ttycallback(GIOChannel *source, GIOCondition condition, void *data)
{
	cout << "ttycallback" << endl;
//	fcntl(((ADT_SerialPort*)data)->fd, F_SETFL, FNDELAY);
	((ADT_SerialPort*)data)->bufferLength=read(((ADT_SerialPort*)data)->fd,((ADT_SerialPort*)data)->buffer, BUFFERSIZE);
	//g_io_channel_read_chars(source, (char*)((ADT_SerialPort*)data)->buffer, BUFFERSIZE, &((ADT_SerialPort*)data)->bufferLength, NULL);
     	((ADT_SerialPort*)data)->onGetData();
   	//g_io_channel_flush(((ADT_SerialPort*)data)->channel,  NULL);
	return 1;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//	Send *data to fd Socket
int ADT_SerialPort::sendData(int fd, const unsigned char *data, int length) const
{
//	g_io_channel_write_chars(channel, (const char*)buffer, bufferLength, NULL, NULL);
	if(write(fd, data, length) < 0)
	{
		cerr << "ERROR writing to socket" << endl;
		return false;
	}
	cout << "data sent" << endl;

//	unsigned char* mybuffer = new unsigned char[BUFFERSIZE];
//	int bl;
//		fcntl(fd, F_SETFL, FNDELAY);
//	while(true)
//	{

//		bl=read(fd,mybuffer, BUFFERSIZE);
//		cout << "Message ("<< bl<<"): ";
//		for(int i=0; i<bl; i++)
//			 cout << mybuffer[i];
//		cout << endl;
////		cout << "MessageHex("<< bl<<"): ";
////		for(int i=0; i<bl; i++)
////			 cout << hex << (int)mybuffer[i] <<", ";
////		cout << endl;
//		if(write(fd, "Message Recived\n\r", 17) < 0)
//		{
//			cerr << "ERROR writing to socket" << endl;
//			return false;
//		}
//	}
//	g_io_channel_flush(channel,  NULL);
 return true;
}
//------------------------------------------------------------------------------
int ADT_SerialPort::sendData(string data) const
{
	return sendData(data.c_str(), data.length());
}
//------------------------------------------------------------------------------
int ADT_SerialPort::sendData(const unsigned char* data, unsigned int length) const
{
	return sendData(fd, data, length);
}
//------------------------------------------------------------------------------
int ADT_SerialPort::sendData(const char* data, unsigned int length) const
{
	return sendData(fd, (unsigned char *)data, length);
}
//------------------------------------------------------------------------------
//	call back function activated when data is recived
void ADT_SerialPort::onGetData()
{	cout << "onGetData()" << endl;
	cout << "Message ("<< bufferLength<<"): "<<endl;

	cout << "MessageHEX ("<< bufferLength<<"): ";
	for(unsigned int i=0; i<bufferLength; i++)
		 cout << hex << "<" <<(int)buffer[i]<<">"<<dec;
	cout << endl;
	for(unsigned int i=0; i<bufferLength; i++)
		 cout << buffer[i]<<dec;
	cout << endl;
//	sendData("Message Recived\n\r", 17);
//	cout << endl;
}
//------------------------------------------------------------------------------
