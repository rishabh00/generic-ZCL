/*******************************************************************************
 * Description:
 * @file : zcl.h
 * @brief : contains zcl sap handlers
 *    
 * Author       - HolyDevil
 *
 *******************************************************************************
 *
 * History
 *
 * Jan/23/2017, Holydevil, Created for training purpose
 *
 ******************************************************************************/

#ifndef ZCL_H
#define ZCL_H

/******************************************************************************
 * Include
 ******************************************************************************/
#include "global.h"
#include "frame_structure.h"
#include "cluster_command.h"
#include "read_attribute_request.h"
#include "read_attribute_response.h"

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
 
/******************************************************************************
 * @brief zcl encoder handler to the other layers
 * @param *_cmd pointer to the structure of type command which will holds the 
 					inforamtion to be stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
int zclEncoder(struct command *cmd,void **packet,int *len);
 
/******************************************************************************
 * @brief zcl encoder handler to the other layers
 * @param *_cmd pointer to the structure of type command which holds the 
 					inforamtion to be stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
 int zclDecoder( struct command *_cmd, void **_packet, int *_len );
#endif
