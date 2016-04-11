/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the implemention of the function that prepare the output file.
*/

#include "Prepaing_The_Output.h"

// The function finds the node which containes the lowest alphabetic destination in an airplanes list and update the pointer to pointer of the previous node of min node. 
airplane* Find_Min (airplane *head_list , airplane **ptr_prev)
{
	airplane *prev_p_list = head_list , *prev = head_list , *min = head_list ; 
	int test = 0;

	head_list = head_list->next ;
	while( head_list != NULL )
	{
		test = strcmp ( min ->destination , head_list -> destination );
		if (test > 0)
		{
			prev = prev_p_list ; 
			min = head_list ; 
			prev_p_list = head_list;
			head_list = head_list ->next ;
		}
		else
		{
			prev_p_list = head_list ; 
			head_list = head_list -> next ;
		}
	}
	*ptr_prev = prev ;
	return (min);
}
		
// The function arranges the airplanes list by alphabetic order and update the pointer to pointer of the head list.
airplane* Arrange_List_By_Alphabetic_Order(airplane **ptr_head_list )
{
	airplane  *min = NULL, *prev = *ptr_head_list ;
	
	if ( (*ptr_head_list) -> next == NULL )
		return ( *ptr_head_list);
	else
	{
		min = Find_Min (*ptr_head_list , &prev);
		if ( min == *ptr_head_list)
		{
			(*ptr_head_list)->next = Arrange_List_By_Alphabetic_Order ( &(     (*ptr_head_list) -> next)  );
			return ( *ptr_head_list);
		}
		else
		{
			prev-> next = min->next;
			min-> next = (*ptr_head_list);
			*ptr_head_list = min;
			(*ptr_head_list) -> next = Arrange_List_By_Alphabetic_Order ( &(    (*ptr_head_list) -> next)  );
			return ( *ptr_head_list );
		}
	}
}

int Create_Output_File ( char* out_file_name , airplane *head_list)
{

	FILE *output_file = NULL ;
	int sum = 0 ;
	airplane *ptr = NULL;

	output_file = fopen (out_file_name , "w");
	if (output_file == NULL )
	{
		printf("FATAL ERROR: Failed opening output file\n");
		Release_P_List (head_list);
		return (1);
	}

	head_list = Arrange_List_By_Alphabetic_Order ( &head_list);

	ptr = head_list;
	while( ptr != NULL )
	{
		fprintf( output_file , "%d pieces of baggage were sent to %s.\n" , ptr -> num_of_pieces , ptr -> destination  );
		sum = sum + ptr -> num_of_pieces ; 
		ptr = ptr -> next ;
	}
	fprintf ( output_file , "In total, %d pieces of baggage were handled." , sum);
	fclose (output_file);
	Release_P_List (head_list);
	return (0);
}