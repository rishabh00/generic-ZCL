/*******************************************************************************
 * Description:
 * @file : read_attribute_request.h
 * @brief : makes and decodes read attribute request packet
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

#ifndef READ_ATTRIBUTE_REQUEST_H
#define READ_ATTRIBUTE_REQUEST_H


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
 * @brief Makes packet for read attribute request
 * @param *_cmd pointer to the structure of type command which holds the 
 				inforamtion to be stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
int readAttributeRequest(struct command *_cmd,void **_packet,int *_len);

/******************************************************************************
 * @brief Decode the packet of read attribute request
 * @param *_cmd pointer to the structure of type command which will holds the 
 					inforamtion stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
 int readAttributeRequestDecoder(struct command *_cmd,void **_packet,int *_len);
#endif
