/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of the functions that close the different resources.
*/

#ifndef CLOSING_PROGRAM
#define CLOSING_PROGRAM

#include "Basic_Types.h"

/*
* The function closes all resources allocated to the program - used in case of FATAL ERROR.
*
* Input:
* -----
* 1. Load_Args - A pointer to a struct defined by us in the Basic_Typed module. 
* 2. Sort_Args - A pointer to a struct defined by us in the Basic_Typed module.
* 3. threads - A pointer to an array of the handles to all the threads that have been created. 
*
* Output:
* -----
* NONE
*/
void Closing_Program_Full ( Load_Args *ptr_load_args , Sort_Args *ptr_sort_args , HANDLE *threads );


/*
* The function releases an airplane lits.
*
* Input:
* -----
* 1. head_list - A pointer to the head of an airplane list.
*
* Output:
* -----
* NONE
*/
void Release_P_List ( airplane *head_list);

/*
* The function closes all resources allocate to the program except the airplanes list - used in case of proper run.
*
* Input:
* -----
* 1. Load_Args - A pointer to a struct defined by us in the Basic_Typed module.
* 2. Sort_Args - A pointer to a struct defined by us in the Basic_Typed module.
* 3. threads - A pointer to an array of the handles to all the threads that have been created.
*
* Output:
* -----
* NONE
*/
void Closing_Program_Partial ( Load_Args *ptr_load_args , Sort_Args *ptr_sort_args , HANDLE *threads );

#endif