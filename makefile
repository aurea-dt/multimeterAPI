#-------------------------------------------------------------------------------
#	AUTOR
#	Aurea Desarrollo Tecnológico.
#	FECHA
#	Notas:
#-------------------------------------------------------------------------------
APPNAME = GTKapp
#Compiler:
	CC=g++

#Compiler flags
	CFLAGS=-c -g -Wall
	INCLUIDES=$(shell pkg-config --cflags goocanvas)
	INCLUIDES+=$(shell xml2-config --cflags)
	LIBS=$(shell pkg-config --libs goocanvas)
	LIBS+=$(shell xml2-config --libs)
#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include
	#libADT= /media/MARIODATOS/Aurea/CPP/Programs/ADTlib-0.0.1
#main function
	mainP= main
	callbacks = My_callbacks
	guiClass = My_guiClass
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o multimeterGUI.o ADT_SerialPort.o ADT_Multimeter.o
	$(CC) -export-dynamic -o $(APPNAME) \
	$(LIBS) \
	$(mainP).o \
	multimeterGUI.o \
	ADT_Multimeter.o \
	ADT_SerialPort.o

ADT_SerialPort.o: ADT_SerialPort.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	ADT_SerialPort.cpp
	
ADT_Multimeter.o: ADT_Multimeter.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	ADT_Multimeter.cpp
	
multimeterGUI.o: multimeterGUI.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	multimeterGUI.cpp
	
mainP.o: $(mainP).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(mainP).cpp
	

	
