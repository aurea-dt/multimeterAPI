//////////////////////////////////////////////////////////////////////////////
//	main.cpp
//	AUTOR
//	Aurea Desarrollo Tecnológico.
//	FECHA
//----------------------------------------------------------------------------
//	DESCRIPCIÓN
//	Notas:	
//		
//gcc -Wall -g main.c -o GTKapp -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "multimeterGUI.h"
using namespace std;
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
        //clutter_init(&argc, &argv);
	gtk_init (&argc, &argv);
	MultimeterGUI gui = MultimeterGUI(argv[1], atoi(argv[2]), argv[3]);
        //clutter_main();
	gtk_main ();
 return 0;
}

//gcc -Wall -g -o tutorial main.c -export-dynamic `pkg-config --cflags --libs gtk+-2.0`
