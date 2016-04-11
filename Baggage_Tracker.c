/*
Authors:		Oron Eliza	 032544264
				Mor Hadar	 302676838

Project:		HW assignment 3

Description:	This file contains the implemention of the function that execute the load and sort function.
*/

#include "Baggage_Tracker.h"
#include "Closing_Program.h"


// Creates a check in array of char and initialize all the slots to be empty.
int Create_Array_Check_In (int num_of_slots , char **ptr_array_check_in )
{
	int i=0 ;

	*ptr_array_check_in = (char*)malloc( num_of_slots * DESTINATION_LENGTH * sizeof(char)  );
	if( *ptr_array_check_in == NULL )
	{
		printf("FATAL ERROR: memory allocation failed");
		return (1);
	}

	for ( i = 0   ;   i < num_of_slots   ;   i++   )
	{
		(*ptr_array_check_in) [ i * ( DESTINATION_LENGTH  ) ] = '\0' ;
	}
	return (0);
}

// Creates a mutex array of handles and initialize all the mutex handles not to be owned by any thread. 
int Create_Array_Mutex ( int num_of_slots , HANDLE **ptr_array_mutex  )
{
	int i=0 ;

	*ptr_array_mutex = (HANDLE*)malloc( num_of_slots * sizeof(HANDLE)  );
	if( *ptr_array_mutex == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (1);
	}

	for (  i=0  ;  i < num_of_slots  ;  i++  )
	{
		(*ptr_array_mutex) [i] = CreateMutex (NULL , 0 ,NULL );
		if ((*ptr_array_mutex) [i] == NULL )
		{
			printf("FATAL ERROR : Creating mutex failed\n ");
			return(1);
		}
	}
	return (0);
}

// Creates a single mutex of new_airplane_mutex and initialize it not to be owned by any thread. 
int Create_New_Airplane_Mutex (HANDLE **new_airplane_mutex)
{
	*new_airplane_mutex = (HANDLE*) malloc ( sizeof(HANDLE) );
	if ( *new_airplane_mutex == NULL)
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (1);
	}
	*(*new_airplane_mutex) = CreateMutex (NULL , 0 , NULL);
	if (*new_airplane_mutex == NULL )
	{
		printf("FATAL ERROR : Creating mutex failed\n ");
		return(1);
	}
	return (0);
}

// Creates two semaphores and intialize them to be with count = 0.		
int Create_Semaphore (int num_of_sorters , HANDLE *semaphores)
{
	
	semaphores[0] = CreateSemaphore (NULL , 0 , num_of_sorters + 1 , NULL ); 
	if ( semaphores[0] == NULL )
	{
		printf("FATAL ERROR: Creating semaphore failed\n");
		return(1);
	}
	semaphores[1] = CreateSemaphore (NULL , 0 , num_of_sorters * 4 , NULL ); 
	if ( semaphores[1] == NULL )
	{
		printf("FATAL ERROR: Creating semaphore failed\n");
		return(1);
	}
	return(0);
}

// Intialize the argument struct for the load func
void Create_Load_Args ( FILE *input_file , HANDLE *array_mutex , HANDLE *semaphores ,  char* array_check_in ,
						int num_of_slots , int num_of_sorters , Load_Args *ptr_load_args )
{
	ptr_load_args -> input_file = input_file ;
	ptr_load_args -> array_mutex = array_mutex ;
	ptr_load_args -> semaphores = semaphores ;
	ptr_load_args -> array_check_in = array_check_in ;
	ptr_load_args -> num_of_slots = num_of_slots ;
	ptr_load_args -> num_of_sorters = num_of_sorters ;
}

// Intialize the argument struct for the sort func
void Create_Sort_Args ( HANDLE *array_mutex , HANDLE *semaphores ,  char* array_check_in , int num_of_slots ,
						airplane **ptr_head_list , HANDLE* new_airplane_mutex , Sort_Args *ptr_sort_args )
{
	ptr_sort_args -> array_mutex = array_mutex ;
	ptr_sort_args -> semaphores = semaphores ;
	ptr_sort_args -> array_check_in = array_check_in ;
	ptr_sort_args -> num_of_slots = num_of_slots;
	ptr_sort_args -> ptr_head_list = ptr_head_list ;
	ptr_sort_args -> new_airplane_mutex = new_airplane_mutex ; 
 }

// Creates an handle for a single thread running the load function.
HANDLE Run_Load_Single_Thread ( int (*func)(Load_Args*) ,Load_Args *ptr_arg)
{
	return CreateThread ( NULL  ,  0  ,  (LPTHREAD_START_ROUTINE)func  ,  ptr_arg  ,  0  ,  NULL  );
}

// Creates an handle for a single thread running the sort function.
HANDLE Run_Sort_Single_Thread ( int (*func)(Sort_Args*) ,Sort_Args *ptr_arg)
{
	return CreateThread ( NULL  , 0  , (LPTHREAD_START_ROUTINE)func , ptr_arg  ,  0  ,  NULL  );
}

// Creates all the thread needed (a single load thread and a num_of_sorters threads).
int Run_Threads ( HANDLE **ptr_threads , int num_of_sorters , Load_Args* ptr_load_args , Sort_Args* ptr_sort_args)
{
	int i = 0;

	*ptr_threads = (HANDLE*)malloc(  (num_of_sorters +1 ) * sizeof(HANDLE)  );
	if( *ptr_threads == NULL )
	{
		printf("FATAL ERROR: Memory allocation failed\n");
		return (1);
	}

	(*ptr_threads) [0] = Run_Load_Single_Thread ( Load_Func, ptr_load_args ) ;
	if ( (*ptr_threads) [0] == NULL )
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",GetLastError()  );
		return (1);
	}
	for ( i = 1 ;   i < num_of_sorters + 1   ;  i++ )
	{
		(*ptr_threads) [i] = Run_Sort_Single_Thread (Sort_Func, ptr_sort_args ) ;
		if ( (*ptr_threads) [i] == NULL )
		{
			printf("FATAL ERROR: Last error 0x%x , Ending program\n",  GetLastError()  );
			return (1);
		}
	}
	return (0);
}

// Checks the returned values of all treads - 1 means a FATAL ERROR accurred and 0 means the thread ran properly. 
int Check_For_Failed_Threads ( HANDLE* threads , int num_of_sorters )
{
	int i = 0 ;
	DWORD   exit_code = 0 ,  *Ip_exit_code = &exit_code ; 
	BOOL success = 0 ;

	for ( i = 0  ; i < num_of_sorters + 1  ;  i++)
	{
		success = GetExitCodeThread ( threads[i] , Ip_exit_code);
		if ( *Ip_exit_code != 0 )
		{
			printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
			return (1);
		}
	}
	return(0);
}

int BaggageTracker ( FILE *input_file , int num_of_slots , int num_of_sorters , airplane **ptr_head_list)
{
	HANDLE semaphores[2] = {NULL , NULL} , *threads = NULL , *array_mutex = NULL , *new_airplane_mutex = NULL ;
	int retval = 0;
	DWORD error_flag = 0 ;
	Load_Args load_args ;
	Sort_Args sort_args ; 
	char *array_check_in = NULL ;
	BOOL success = 0 ;


	retval = Create_Array_Check_In ( num_of_slots , &array_check_in );
	if (retval == 1 )
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}

	retval = Create_Array_Mutex ( num_of_slots , &array_mutex );
	if (retval == 1 )
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}
	
	retval = Create_New_Airplane_Mutex ( &new_airplane_mutex );
	if (retval == 1 )
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}

	retval = Create_Semaphore ( num_of_sorters , semaphores ) ;
	if (retval == 1 )
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}

	Create_Load_Args ( input_file , array_mutex ,semaphores, array_check_in , num_of_slots  ,num_of_sorters ,  &load_args );
	Create_Sort_Args ( array_mutex , semaphores , array_check_in , num_of_slots , ptr_head_list , new_airplane_mutex , &sort_args);

	retval = Run_Threads( &threads , num_of_sorters , &load_args , &sort_args );
	if (retval == 1 )
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}
	Sleep (10000);
	success = ReleaseSemaphore ( semaphores[0] , num_of_sorters + 1 , NULL );//Ready - Set - Gooooooooo
	if ( success == 0 )
	{
		printf("FATAL ERROR: Releasing semaphore failed\n");
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}
	
	error_flag = WaitForMultipleObjects( num_of_sorters + 1 , threads , 1 , INFINITE );
	if (error_flag == WAIT_FAILED)
	{
		printf("FATAL ERROR: Last error 0x%x , Ending program\n",   GetLastError()  );
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}

	retval = Check_For_Failed_Threads (threads , num_of_sorters);
	if (retval == 1)
	{
		Closing_Program_Full (&load_args , &sort_args , threads );
		return (1);
	}
	
	Closing_Program_Partial (&load_args , &sort_args , threads );
	return (0);	
}