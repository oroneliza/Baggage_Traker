/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the implementation of the functions that performs the main logic of the program -
				The loading assingment and the sorting assinment.
*/

#include "Load_And_Sort_Logic.h"


///////// The load logic /////////

// Arranges the line that was read from the input file to be NULL terminated and counts the number of 
// destinations in the line.
int Arrange_Line (FILE* input_file , char* line)
{
	int line_len = 0;

	if ( feof(input_file) == 0 )
	{
		line_len = strlen (line);
		line[ line_len - 1 ] = '\0';
	}
	return (   (strlen(line) + 1 ) / 4   );
}

//Copies the i destination from the line to the 'destinations' string (and fixing it to be NULL terminated). 
void Copy_The_i_Destination_From_Line (char *line , char *destination, int i )
{
	int j = 0 ;
	for ( j = 0  ;  j < DESTINATION_LENGTH  ;  j++ )
	{
		destination[j] = toupper( line[ (DESTINATION_LENGTH + 1)*i + j ]   ); 
	}
	destination[j] = '\0' ;
	return;
}

//Copies the 'destination' string to the i slot in the check in array. 
void Put_The_Baggage_At_The_i_Slot ( char* array_check_in , char* destination, int i)
{
	int j=0;
	
	for (j =0 ;  j < DESTINATION_LENGTH  ; j++)
	{
		array_check_in [   (DESTINATION_LENGTH * i) + j  ] = destination[j] ;
	}
	return;
}
	
//Finds an available slot in the check in array and place the baggage inside. 
//If all slots are taken - waits for 10ms and then performs the function again until finding an available slot.
int Place_Baggage_In_An_Available_Slot ( char *destination , char *array_check_in , HANDLE *array_mutex , int num_of_slots)
{
	int i = 0 , retval = 0 , all_slots_are_taken = 0 ;
	DWORD slot_is_locked = 0 ;
	BOOL error_flag = 0 ;
	
	for ( i = 0  ;  i < num_of_slots  ;  i++ )
	{
		slot_is_locked = WaitForSingleObject ( array_mutex[i] , 0 );
		if (slot_is_locked == WAIT_FAILED )
		{
			printf("FATAL ERROR: Last error = 0x%x , Ending program\n"  ,  GetLastError );
			return(1);
		}

		if (slot_is_locked == WAIT_TIMEOUT )
		{
			retval = WaitForSingleObject ( array_mutex[i] , INFINITE );
			if ( retval == 1)
			{
				printf("FATAL ERROR: Last error = 0x%x , Ending program\n"  ,  GetLastError );
				return(1);
			}
			Put_The_Baggage_At_The_i_Slot ( array_check_in , destination , i);
			error_flag = ReleaseMutex ( array_mutex[i] ) ;
			if ( error_flag == 0 )
			{
				printf("FATAL ERROR: Releasing mutex failed\n");
				return (1);
			}
			return(0);


		}
		else
		{
			if ( array_check_in[ (DESTINATION_LENGTH ) * i ] == '\0' )
			{
				Put_The_Baggage_At_The_i_Slot ( array_check_in , destination , i);
				error_flag = ReleaseMutex ( array_mutex[i] ) ;
				if ( error_flag == 0 )
				{
					printf("FATAL ERROR: Releasing mutex failed\n");
					return (1);
				}
				return(0);
			}
			else
			{
 				all_slots_are_taken++ ;
				error_flag = ReleaseMutex ( array_mutex[i] ) ;
				if ( error_flag == 0 )
				{
					printf("FATAL ERROR: Releasing mutex failed\n");
					return (1);
				}
			}
		}
		
		if ( all_slots_are_taken == num_of_slots )
		{
			all_slots_are_taken = 0 ;
			i = -1 ;
			Sleep (10) ;
		}
	}
}

int Load_Func ( Load_Args *load_args)
{
	char destination [ DESTINATION_LENGTH + 1 ] , line [LINE_LENGTH] ;
	int retval = 0 , num_of_destinations_in_line = 0 , i = 0;
	BOOL success = 0 ; 
	DWORD error_flag;

	error_flag = WaitForSingleObject ( load_args -> semaphores[0] , INFINITE );
	if (error_flag == WAIT_FAILED)
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
		return (1);
	}
	
	while ( feof (load_args -> input_file) == 0 )
	{
		fgets( line  ,  LINE_LENGTH  ,  load_args -> input_file );
		num_of_destinations_in_line = Arrange_Line ( load_args -> input_file , line );

		for ( i = 0   ;  i < num_of_destinations_in_line  ;  i++  )
		{
			Copy_The_i_Destination_From_Line ( line  , destination , i );
			retval = Place_Baggage_In_An_Available_Slot (   destination ,   load_args -> array_check_in   ,  
				                                            load_args -> array_mutex  ,  load_args -> num_of_slots   );
			if (retval == 1 )
				return(1);
		}
	}
	success = ReleaseSemaphore( load_args -> semaphores[1] , ( load_args -> num_of_sorters ) * 4 , NULL );
	if ( success == 0 )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
		return (1);
	}
	return(0);
}



/////////  The sort logic /////////

//Evacuates the i slot in the check in array and remembers the baggage destination in the string 'str'. 
void Take_The_Baggage_From_The_i_Slot ( char* str ,  char *array_check_in  ,  int i )
{
	int j = 0 ;
	
	for ( j = 0  ;  j < DESTINATION_LENGTH   ;  j++ )
	{
		str[j] = array_check_in [  ( (DESTINATION_LENGTH  ) * i ) + j  ];
	}
	str[j] = '\0' ;
	array_check_in [  ( DESTINATION_LENGTH  ) * i  ] = '\0' ; 
	return;
}

// The function addes a single baggage to a temp liked_list of baggages. 
// Every sort thread ownes is own temporary list of baggages until finishing scaning the check in array once.
int Add_Baggage(  airplane **ptr_head_list  ,  char *str  )
{
	if (*ptr_head_list == NULL )
	{
		*ptr_head_list = (airplane*)malloc(  sizeof(airplane)  );
		if (*ptr_head_list == NULL )
		{
			printf("FATAL ERROR: Memory allocation failed\n");
			return (1);
		}
		(*ptr_head_list) -> num_of_pieces_mutex = NULL ;
		(*ptr_head_list) -> num_of_pieces = 1 ;
		(*ptr_head_list) -> next = NULL ;
		strcpy( (*ptr_head_list) -> destination , str);
		return(0);
	}
	else if (  strcmp( (*ptr_head_list) -> destination , str) == 0 )
	{
		(*ptr_head_list)->num_of_pieces ++ ;
		return (0);
	}
	else
		return ( Add_Baggage ( &( (*ptr_head_list)->next ) , str) );
}

//Merges the temporary baggages list of a single sort thread with the main airplanes list.
int Add_Temp_List_To_Airplane_List ( airplane *ptr_head_temp_list , airplane **ptr_head_list , HANDLE *new_airplane_mutex)
{
	airplane *temp_p = ptr_head_temp_list , **ptr = ptr_head_list , *new_plane_p = NULL ;
	DWORD error_flag = 0 ;
	BOOL success = 0 ;
	int update_flag = 0;

	while( temp_p  !=  NULL)
	{
		while ( *ptr != NULL )
		{
			if (   strcmp( (*ptr) -> destination , temp_p -> destination ) == 0  )
			{
				error_flag = WaitForSingleObject(  *( (*ptr)-> num_of_pieces_mutex ) , INFINITE ) ;
				if( error_flag == WAIT_FAILED)
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
					return (1);
				}
				(*ptr)-> num_of_pieces  =  (*ptr)-> num_of_pieces   +   temp_p-> num_of_pieces ;
				success = ReleaseMutex(  *( (*ptr)-> num_of_pieces_mutex ) );
				if (success == 0 )
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
					return (1) ;
				}
				temp_p = temp_p-> next ; 
				ptr = ptr_head_list ;
				update_flag = 1;
				break;
			}
			else
			{
				ptr = &( (*ptr)-> next ) ;
			}
		}
		if ( update_flag == 1 )
		{
			update_flag = 0;
			continue;
		}
		error_flag = WaitForSingleObject(   *new_airplane_mutex  ,  0  );
		if (error_flag == WAIT_FAILED)
		{
			printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
			return (1);
		}

		if (error_flag == WAIT_TIMEOUT)
		{
			Sleep (10) ;
		}
		else
		{
 			new_plane_p = (airplane*)malloc(   sizeof(airplane)  ) ;
			if ( new_plane_p == NULL )
			{
				printf("FATAL ERROR: Memory allocation failed\n") ;
				success = ReleaseMutex(  *new_airplane_mutex );
				if (success == 0 )
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
					return (1);
				}
				return (1) ;
			}
			strcpy ( new_plane_p->destination , temp_p-> destination ) ;
			new_plane_p-> num_of_pieces = temp_p-> num_of_pieces ; 
			new_plane_p-> next = NULL ;
			new_plane_p ->num_of_pieces_mutex = (HANDLE*)malloc(   sizeof(HANDLE)  ) ;
			if ( new_plane_p ->num_of_pieces_mutex == NULL )
			{
				printf("FATAL ERROR: Memory allocation failed\n") ;
				success = ReleaseMutex(  new_airplane_mutex );
				if (success == 0 )
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
					return (1);
				}
				return (1) ;
			}
			*( new_plane_p-> num_of_pieces_mutex ) = CreateMutex(  NULL  ,  0  ,  NULL ) ;
			if ( *(new_plane_p-> num_of_pieces_mutex) == NULL )
			{
				printf("FATAL ERROR: Creating mutex failed\n") ;
				free(new_plane_p) ;
				success = ReleaseMutex(  *new_airplane_mutex );
				if (success == 0 )
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
					return (1);
				}
				return (1) ;
			}
			*ptr = new_plane_p ;
			success = ReleaseMutex(  *new_airplane_mutex );
			if (success == 0 )
			{
				printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  ) ;
				return (1);
			}
			temp_p = temp_p-> next ; 
			ptr = ptr_head_list ;
		}
	}
	return (0);
}
				
int Sort_Func ( Sort_Args *sort_args)
{
	
	airplane *temp_ptr_list = NULL ;
	int i = 0 , j = 0 , work = 0 , ceased_work = 0 , retval = 0 , baggage_added = 0 ;
	char str [DESTINATION_LENGTH + 1] ;
	DWORD ready_set_go = 0 , mutex_flag = 0 , load_flag = 0 ;
	BOOL success = 0 ;

	ready_set_go = WaitForSingleObject ( sort_args -> semaphores[0] , INFINITE );
	if (ready_set_go == WAIT_FAILED)
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
		return (1);
	}

	load_flag = WaitForSingleObject( sort_args -> semaphores[1] , 0);
	if (load_flag == WAIT_FAILED)
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
		return (1);
	}

	while ( (load_flag != WAIT_OBJECT_0)  ||   ( ceased_work )   )
	{
		work = 0;
		for ( i = 0  ;  i < (sort_args -> num_of_slots)  ;  i++ )
		{
			mutex_flag = WaitForSingleObject ( sort_args ->array_mutex[i] , 0 );
			if (mutex_flag == WAIT_FAILED)
			{
				printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
				return (1);
			}
			if ( mutex_flag == WAIT_OBJECT_0 )
			{
				if (sort_args ->  array_check_in [ (DESTINATION_LENGTH ) * i ]   !=   '\0'   )
				{
					Take_The_Baggage_From_The_i_Slot ( str , sort_args ->array_check_in , i);
					baggage_added = 1;
					work = 1;
				}
				success = ReleaseMutex( sort_args -> array_mutex[i]  );
				if ( success == 0 )
				{
					printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
					return (1);
				}
				if (baggage_added == 1)
				{
					baggage_added = 0 ;
					retval =  Add_Baggage ( &temp_ptr_list , str );
					if (retval == 1)
					{
						Release_P_List ( temp_ptr_list );
						return (1);
					}
				}
			}
		}
		if ( temp_ptr_list != NULL )
		{
			retval = Add_Temp_List_To_Airplane_List ( temp_ptr_list , sort_args-> ptr_head_list , sort_args-> new_airplane_mutex );
			Release_P_List (temp_ptr_list);
			temp_ptr_list = NULL;
		}

		if (retval == 1)
			return(1);

		if ( ceased_work == 1 )
			break;

		load_flag = WaitForSingleObject( sort_args -> semaphores[1] , 0);
		if (load_flag == WAIT_FAILED)
		{
			printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
			return (1);
		}
		if ( (load_flag == WAIT_OBJECT_0) )
			ceased_work = 1 ;
		if ( work == 0 )
		{
			Sleep(10);
		}

		
	}
	return(0);
}
