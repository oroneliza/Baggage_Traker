/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the implementation of the function that performs the arguments check.
*/

#include "Argument_Check.h"

int Checks (  char* argv[] , FILE** ptr_input_file )
{
	*ptr_input_file = fopen (argv[1] , "r" );
	if ( *ptr_input_file == NULL)
	{
		printf("FATAL ERROR: Failed opening file\n" );
		return(1);
	}
	return (0);
}
