/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the declarations of new types and constants.
*/

#ifndef BASIC_TYPES
#define BASIC_TYPES

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define DESTINATION_LENGTH 3
#define LINE_LENGTH 128

/*
* This struct represents a single airplane. The fields are as follows:
* 1. destination - An array of char that contains the destination of the single airplane.
* 2. num_of_pieces_mutex - A pointer to a mutex handle controlling the access to the num of baggages of the airplaine.
* 3. num_of_pieces - An integer of the num of the baggages that must be loaded on the single airplane.
* 4. next - A pointer to airplane struct. Will point to the next node in the liked list.  
*/ 
typedef struct airplane
{
	char destination[DESTINATION_LENGTH+1];
	HANDLE *num_of_pieces_mutex;
	int num_of_pieces ;
	struct airplane *next;
}airplane;

/*
*Load_Args - A struct of arguments. The fields are as follows:
*1. input_file - A pointer to input file
*2. array_mutex - A pointer to the mutexes array.
*			      Every single mutex allows / does not allow access to the i slot in the check in array.
*3. semaphores - A pointer to the semaphores array.
*			     semaphore[0] - used to implement the ready - set - go logic.
*	     	     semaphore[1] - used by the load thread to announce to the other threads that he has finished load all the baggages in the input file.
*4. array_check_in - A pointer to array of char.
*				     Every trio of chars is a single slot.
*5. num_of_slots - An integer of the number of slots.
*6. num_of_sorters - An integer of the number of the sorters.
*/
typedef struct Load_Args
{
	FILE *input_file;
	HANDLE *array_mutex , *semaphores ;
	char *array_check_in ;
	int num_of_slots , num_of_sorters;
}Load_Args;

/*
*Sort_Args - A struct of arguments. The fields are as follows:
*1. array_mutex - A pointer to the mutexes array.
*			      Every single mutex allows / does not allow access to the i slot in the check in array.
*2. semaphores - A pointer to the semaphores array.
*				 semaphore[0] - used to implement the ready - set - go logic.
*				 semaphore[1] - used by the load thread to announce to the other threads that he has finished load all the baggages in the input file
*3. array_check_in - A pointer to array of char.
*				   	 Every trio of char is a single slot.
*4. num_of_slots - An integer of the number of slots.
*5. ptr_head_list - A pointer to pointer to the main airplanes list.
*6. new_airplane_mutex - A pointer to a mutex controlling the access for creating a
*					     a new node at the airplanes lits's
*/
typedef struct Sort_Args
{
	HANDLE *array_mutex , *semaphores ; 
	char *array_check_in;
	int num_of_slots;
	airplane **ptr_head_list;
	HANDLE *new_airplane_mutex;

}Sort_Args;

#endif