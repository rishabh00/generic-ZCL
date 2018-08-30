/******************************************************************************
 * Description:
 *    
 * A description of the entire functional block. This description should include
 * design information and details which describe the design in more detail than 
 * that found in the requirement.
 * 
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/01/2017, HolyDevil, Created for training purpose
 *
 ******************************************************************************/
/******************************************************************************
 *	Includes
 ******************************************************************************/
#include "global.h"
#include "cluster_id.h"
#include "on_off.h"

/*****************************************************************************
 * 	Defines
 *****************************************************************************/
/* None */

/*****************************************************************************
 * Function Prototypes
 ****************************************************************************/
int validateClusterId ( uint16_t _clusterId, uint16_t _arr )
{
	switch ( _clusterId ) {								/* switch to the given cluster and then validates the attributes */
		case ZCL_CLUSTERID_ONOFF :
			if( validateOnOffAttr ( _arr ) == INVALID_ATTRIBUTE )
				return INVALID_ATTRIBUTE;		
			break;
		/*case ZCL_CLUSTERID_LEVEL_CONTROL:		validateLevelControlAttr(_arr,_attrLen);		break
		case ZCL_CLUSTERID_DEVICE_TEMPERATURE:	validateDeviceTemperatureAttr(_arr,_attrLen);	break;
			  case ZCL_CLUSTERID_IDENTIFY:			validateIndentifyAttr(_arr);					break;
			  case ZCL_CLUSTERID_BASIC:				validateOnOffAttr(_arr);						break;
			  case ZCL_CLUSTERID_POWER:				validatePowerAttr(_arr);						break;
			  case ZCL_CLUSTERID_ALARM:				validateAlarmAttr(_arr);						break;
			  case ZCL_CLUSTERID_TIME:				validateTimeAttr(_arr);							break;
			  case ZCL_CLUSTERID_LOCATION:			validateLocationAttr(_arr);						break;
			  case ZCL_CLUSTERID_COMMISIONING:		validateCommisioningAttr(_arr);					break;
			  case ZCL_CLUSTERID_ANALOG_INPUT:		validateAnalogInputAttr(_arr);					break;
			  case ZCL_CLUSTERID_ANALOG_OUTPUT:		validateAnalogOutputAttr(_arr);					break;
			  case ZCL_CLUSTERID_BINARY_INPUT:		validateBinaryInputAttr(_arr);					break;
			  case ZCL_CLUSTERID_BINARY_OUTPUT:		validateBinaryOutputAttr(_arr);					break;
			  */
		default : 	
			return INVALID_CLUSTERID;
	}
	return VALID_DATA;	
}

