/*******************************************************************************
 * 
 * Description:
 * @file : read_attribute_response.h
 * @brief : makes and decodes read attribute response packet
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

#ifndef READ_ATTRIBUTE_RESPONSE_H
#define READ_ATTRIBUTE_RESPONSE_H

/******************************************************************************
 * Include
 ******************************************************************************/
#include "global.h"
#include "frame_structure.h"

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
 
/******************************************************************************
 * @brief Makes read attribute response packet
 * @param *_cmd pointer to the structure of type command which holds the 
 					inforamtion to be stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
int readAttributeResponse ( struct command *_cmd, void **_packet, int *_len );

/******************************************************************************
 * @brief Decodes read attribute response packet
 * @param *_cmd pointer to the structure of type command which will holds the 
 					inforamtion stored in packet
 * @param **_packet pointer to the pointer of packet
 * @param *_length pointer to the length of packet
 * @return SUCCESSFUL
 ********************************************************************************/
 int readAttributeResponseDecoder ( struct command *_cmd, void **_packet, int *_len );

#endif
