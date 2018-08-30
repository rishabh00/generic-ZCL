/*******************************************************************************
 * Description:
 * @file : frame_structure.h
 * @brief : frame structure enums
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/23/2017, Holydevil, Created for training purpose
 *
 ******************************************************************************/

#ifndef FRAMESTRUCTURE_H
#define FRAMESTRUCTURE_H

/*******************************************************************************
 * Enums
 *******************************************************************************/
typedef enum {
	READ_ATTRIBUTE_ID							= 0X00,
	READ_ATTRIBUTE_RESPONSE_ID					= 0X01,
	WRITE_ATTRIBUTE_ID							= 0X02,
	WRITE_ATTRIBUTE_UNDIVIDED_ID				= 0X03,
	WRITE_ATTRIBUTE_RESPONSE_ID					= 0X04,
	WRITE_ATTRIBUTE_NO_RESPONSE_ID				= 0X05,
	CONFIGURE_REPORTING_ID						= 0X06,
	CONFIGURE_REPORTING_RESPONSE_ID				= 0X07,
	READ_REPORTING_CONFIGURATION_ID				= 0X08,
	READ_REPORTING_CONFIGURATION_RESPONSE_ID	= 0X09,
	REPORT_ATTRIBUTES_ID						= 0X0a,
	DEFAULT_RESPONSE_ID							= 0X0b,
	DISCOVER_ATTRIBUTE_ID						= 0X0c,
	DISCOVER_ATTRIBUTE_RESPONSE_ID				= 0X0d,
	READ_ATTRIBUTE_STRUCTURED_ID				= 0X0e,
	WRITE_ATTRIBUTE_STRUCTURED_ID				= 0X0f,
	WRITE_ATTRIBUTE_STRUCTURED_RESPONSE_ID		= 0X10
} commandIdentifier;

typedef enum {
	FRAME_TYPE_ENTIRE_PROFILE			=0b00000000,
	FRAME_TYPE_CLUSTER_SPECIFIC			=0b00000001,
	INCLUDE_MANUFACTURER_SPECIFIC		=0b00000100,
	EXCLUDE_MANUFACTURER_SPECIFIC		=0b00000000,
	CLIENT_TO_SERVER					=0b00000000,
	SERVER_TO_CLIENT					=0b00001000,
	DISABLE_DEFAULT_RESPONSE			=0b00010000,
	ENABLE_DEFAULT_RESPONSE				=0b00000000,
} frameControl;


#endif  
