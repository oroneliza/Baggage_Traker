/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of the function that execute the load and sort function.
*/

#ifndef BAGGAGE_TRACKER
#define BAGGAGE_TRACKER


#include "Basic_Types.h"
#include "Load_And_Sort_Logic.h"

/*
* The function execute the load and sort function by creating and intializing needed arguments, creating the threads with these arguments
* and implements the head of the airplanes linked list to the main function.
*
* Input:
* -----
* 1. input_file - A pointer to the input file.
* 2. num_of_slots - An integer of the num of slots requested by the user.
* 3. num_of_sorters - An integer of the num of sorters requested by the user.
* 4. ptr_head_list - A pointer to pointer of the main airplanes list.
*
* Output:
* -----
* 1. Integer - Returns 1 if a FATAL ERROR occurred And 0 otherwise .
*			   ***** In case of FATAL ERROR the function closes all reasources.
*			   ***** In case of proper run the function closes all resources except the main airplanes list.
*/
int BaggageTracker ( FILE *input_file , int num_of_slots , int num_of_sorters , airplane **ptr_head_list);

#endif