/*******************************************************************************
 * 
 * Description:
 * @file : cluster_command.h
 * @brief :  Makes packet for cluster specific command 
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


#ifndef CLUSTER_COMMAND_H
#define CLUSTER_COMMAND_H

/******************************************************************************
 * Include
 ******************************************************************************/
#include "global.h"
#include "frame_structure.h"
#include "cluster_id.h"

/******************************************************************************
 * Function Prototype
 ******************************************************************************/

/******************************************************************************
 * @brief Makes cluster specific Packet
 * @param *_cmd pointer to the structure of type command which holds the 
 inforamtion to be stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
int makeClusterSpecificPacket ( struct command *_cmd, void **_packet, int *_length );

/******************************************************************************
 * @brief Decodes cluster specific Packet
 * @param command *_cmd pointer to the structure of type command which will holds the 
 inforamtion stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/

int decodeClusterSpecificPacket ( struct command *_cmd, void **_packet, int *_length );
#endif
