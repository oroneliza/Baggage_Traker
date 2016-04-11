/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of the function that performs the arguments check.
*/


#ifndef ARGUMENT_CHECK
#define ARGUMENT_CHECK


#include <Windows.h>
#include <stdio.h>

/*
* The function checks if the input file can be opened.
*
* Input:
* -----
* 1. argv - An array of 4 strings contaning the user input arguments.
*           The function uses only the first argument - The name of the input file 
* 2. ptr_input_file - A pointer to the input file (used as an additional output).
*
* Output:
* -----
* 1. integer - Returns 1 if a FATAL ERROR occurred (If the file can't be opened) And 0 otherwise .
*/
int Checks (  char* argv[] , FILE** ptr_input_file );

#endif 