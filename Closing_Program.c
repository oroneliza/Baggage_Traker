/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the implemention of the functions that close the different resources.
*/
#include "Closing_Program.h"


void Closing_Program_Partial ( Load_Args *ptr_load_args , Sort_Args *ptr_sort_args , HANDLE *threads )
{
	int i = 0 ;
	
	
	fclose (ptr_load_args -> input_file );
	
	for ( i = 0  ;  i < ptr_load_args -> num_of_slots ; i++ )
	{
		if (ptr_load_args -> array_mutex[i] != NULL )//release??
			CloseHandle (ptr_load_args -> array_mutex[i] ) ;
	}
	
	for ( i = 0   ;  i < ptr_load_args ->num_of_sorters + 1   ;   i++ )
	{
		if (threads[i] != NULL )
			CloseHandle( threads[i] );
	}

	for (i=0  ; i<2 ; i++ )
	{
		if (ptr_load_args->semaphores[i] != NULL)//release??
			CloseHandle ( ptr_load_args->semaphores[i]);
	}

	if ( *(ptr_sort_args -> new_airplane_mutex) != NULL )//release??
	{
		CloseHandle( *(ptr_sort_args -> new_airplane_mutex) );
		free ( ptr_sort_args ->new_airplane_mutex);
	}
	
	if (ptr_load_args -> array_mutex != NULL )
		free ( ptr_load_args -> array_mutex );

	if ( ptr_load_args ->array_check_in != NULL )
		free ( ptr_load_args ->array_check_in );

	if ( threads != NULL )
		free( threads );
}
	
void Release_P_List ( airplane *head_list)
{
	if ( head_list == NULL)
		return;
	if ( head_list -> next != NULL )
	{
		Release_P_List (head_list ->next );
		if (head_list -> num_of_pieces_mutex != NULL )
			CloseHandle( *( head_list -> num_of_pieces_mutex ) );
		free( head_list );
	}
	else
	{
		if (head_list -> num_of_pieces_mutex != NULL )
			CloseHandle( *( head_list -> num_of_pieces_mutex ) );
		free( head_list );
	}
	return;
}
			
void Closing_Program_Full ( Load_Args *ptr_load_args , Sort_Args *ptr_sort_args , HANDLE *threads )
{
	Closing_Program_Partial (  ptr_load_args  ,  ptr_sort_args  ,  threads  );
	Release_P_List (  *ptr_sort_args -> ptr_head_list   );
	return;
}