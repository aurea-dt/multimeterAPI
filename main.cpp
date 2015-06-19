////////////////////////////////////////////////////////////////////////////////
/**
*	@file	main.cpp
*	@author	Mario Chirinos Colunga
*	@date	
*	@note	main file template
*/
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include "ADT_Multimeter.h"
using namespace std;
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if(argc < 0)
	{
		cerr << "too few arguments" << endl;
		return EXIT_FAILURE;
	}
	ADT_Multimeter multimeter = ADT_Multimeter(argv[1], atoi(argv[2]), argv[3]);
	
	GMainLoop* main_loop = NULL;
	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run(main_loop);
 return 0;
}
//------------------------------------------------------------------------------
