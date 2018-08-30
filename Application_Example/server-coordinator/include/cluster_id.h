/*******************************************************************************
 * Description:
 * @file : cluster_id.h
 * @brief : validates cluster id and attribute id
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/23/2017, Holydevil, Created for training purpose
 *
 ******************************************************************************/

#ifndef CLUSTER_ID_H
#define CLUSTER_ID_H

/*******************************************************************************
 * Define
 ******************************************************************************/
#define SUPPORTED_CLUSTER						15

/******************************************************************************
 * Enums
 ******************************************************************************/
typedef enum
{
	ZCL_CLUSTERID_BASIC 					= 0x0000,
	ZCL_CLUSTERID_POWER						= 0x0001,
	ZCL_CLUSTERID_DEVICE_TEMPERATURE		= 0x0002,
	ZCL_CLUSTERID_IDENTIFY					= 0x0003,
	ZCL_CLUSTERID_ONOFF						= 0x0006,
	ZCL_CLUSTERID_LEVEL_CONTROL				= 0x0008,
	ZCL_CLUSTERID_ALARM						= 0x0009,
	ZCL_CLUSTERID_TIME						= 0x000a,
	ZCL_CLUSTERID_LOCATION					= 0x000b,
	ZCL_CLUSTERID_COMMISIONING				= 0x0015,
	ZCL_CLUSTERID_ANALOG_INPUT				= 0x000c,
	ZCL_CLUSTERID_ANALOG_OUTPUT				= 0x000d,
	ZCL_CLUSTERID_BINARY_INPUT				= 0x000f,
	ZCL_CLUSTERID_BINARY_OUTPUT				= 0x0010
} ZCL_Cluster_Id;

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
 
/******************************************************************************
** @brief Checks that the given atribute belongs to the given cluster and checks 
				as well that the cluster is defined or not
 * @param _clusterId holds the cluster Id
 * @param _attr holds the attribute Id
 * @return SUCCESSFUL
**
******************************************************************************/

int validateClusterId ( uint16_t _clusterId, uint16_t _attr );


#endif  /* '#endif' of CLUSTERID_H */
