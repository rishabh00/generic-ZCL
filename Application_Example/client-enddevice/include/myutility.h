/*******************************************************************************
 * 
 * Description:
 * @file comman.h	
 * @brief:  Defination of structure variables and decleration functions which are 
 *          used in admin profile.
 *    
 * Author       - Rishabh S Sheth
 *
 *******************************************************************************
 *
 * History
 *                      
 * Jan/24/2017, Rishabh S, Created 
 *
 ******************************************************************************/

#ifndef comman_h
#define comman_h


/*******************
 * Includes
 *******************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/***************************************
 *Global Variable
 ***************************************/
extern void *device_data[5];

typedef struct
{
	int status;
	char name[20];
}BulbData;
/***************************************
 *Function Decleration
 ***************************************/

void flush( void );

void clrscr( void );

void end( void );

#endif
