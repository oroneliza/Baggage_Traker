/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of the functions that performs the main logic of the program -
				The loading assingment and the sorting assingment.
*/

#ifndef LOAD_AND_SORT_LOGIC
#define LOAD_AND_SORT_LOGIC


#include <string.h>
#include <tchar.h>
#include "Basic_Types.h"
#include "Closing_Program.h"

/*
* The fuction performs the loading assingment - reads the various destinations from the input file and placing 
* them in the available slots in the arrary check in.
*
* Input:
* -----
* 1. Load_Args - A pointer to a struct defined by us in the Basic_Typed module.
*	 
* Output:
* -----
* 1. Integer - Returns 1 if a FATAL ERROR occurred And 0 otherwise .
*/
int Load_Func ( Load_Args *load_args);


/*
* The fuction performs the sorting assingment - scaning the check in array constantly and creates a temporary linked list of airplanes
* to every destination mentioned in the input file with the total number of baggages that must to be loaded on the airplane, then merge the temporary linked list
* with the main airplanes list.
*
* Input:
* -----
* 1. Sort_Args - A pointer to a struct defined by us in the Basic_Typed module.
*
* Output:
* -----
* 1. Integer - Returns 1 if a FATAL ERROR occurred And 0 otherwise .
*/
int Sort_Func ( Sort_Args *sort_args);

#endif