/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of the function that prepare the output file.
*/
#ifndef PREPARING_THE_OUTPUT
#define PREPARING_THE_OUTPUT


#include "Basic_Types.h"
#include "Closing_Program.h"

/*
* The function prepare the output file and releases the main airplanes list.
*
* Input:
* -----
* 1. out_file_name - A string contains the output file name requested by the user.
* 2. head_list - A pointer to the head of the main airplane list.
*
* Output:
* -----
* 1. Integer - Returns 1 if a FATAL ERROR occurred and 0 otherwise .
*/
int Create_Output_File ( char* out_file_name , airplane *head_list);

#endif