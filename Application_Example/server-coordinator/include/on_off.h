/*******************************************************************************
 * Description:
 * @file : on_off.h
 * @brief : on off cluster specific enums and commands
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/23/2017, Holydevil, Created for training purpose
 *
 ******************************************************************************/

#ifndef ON_OFF_H
#define ON_OFF_H

/******************************************************************************
 * Include
 ******************************************************************************/
#include "global.h"

/******************************************************************************
 * Enums
 ******************************************************************************/
typedef enum
{
	ZCL_ATTRIBUTE_ID_ONOFF_ONOFF			= 0x0000,
} attribute_id_onoff_cluster_e;

typedef enum
{
	ZCL_ATTRIBUTE_DATATYPE_ONOFF_ONOFF		= 0x10,
} attribute_data_type_onoff_cluster_e;

typedef enum
{
	ZCL_ATTRIBUTE_SIZE_ONOFF_ONOFF			= 1,
} attribute_size_onoff_cluster_e;

typedef enum
{
	ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_OFF			= 0x00,
	ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_ON				= 0x01,
	ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_TOGGLE			= 0x02,
} cluster_specific_command_onoff_cluster_e;

/******************************************************************************
 * Structure
 ******************************************************************************/
struct on_off{
	bool onOff;
} onoff_t;


/******************************************************************************
 * Function Prototype
 ******************************************************************************/
int validateOnOffAttr ( uint16_t _arr );  

#endif
