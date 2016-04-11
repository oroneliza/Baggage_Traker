/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the main function of the program.
*/

#define CRT_SECURE_NO_WARNINGS

#include "Baggage_Tracker.h"
#include "Argument_Check.h"
#include "Prepaing_The_Output.h"
#include "Closing_Program.h"


int main ( int argc , char* argv[])
{
	int retval = 0;
	airplane *head_list = NULL ;
	FILE *input_file = NULL ;

	retval = Checks (argv , &input_file);
	if (retval == 1 )
		return (1);

	retval = BaggageTracker (input_file , atoi(argv[2]) , atoi( argv[3]) ,&head_list );
	if (retval == 1 )
		return (1);
	
	retval = Create_Output_File ( argv[4] , head_list );
	if (retval == 1 )
		return (1);

	return(0);
}