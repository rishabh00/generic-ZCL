/*****************************************************************************
 * Description:
 * @file nwk.h 
 * @brief: Defination & decleration for network management 
 *    
 * Author       - Rishabh S Sheth
 *
 *******************************************************************************
 *
 * History
 *                      
 * March/16/2017, Rishabh S, Created 
 *
 ******************************************************************************/

#ifndef __NWK__
#define __NWK__


/*******************
 * Includes
 *******************/
#include "global.h"

/********************
 * Defination 
 ********************/
typedef struct 
{
    uint16_t device_id;
    uint16_t packet_length;
    uint8_t packet[500];
}NwkMsg_t;

/************************
 * Function Prototypes
 ************************/


/**@brief  This fuction is used to initialize network layer it will create required thread and
            set various global parameter for future refrence.
 
 * @param in type ;type reprensent which type of layer you want to initialize
                   type = 1 is for server
                   type = 2 is for client
 * @param out init;function will start rquired thread and initialize globle variable 
 
* @return  function return 1 after successfully connection of network;
*/
int NWK_Init( int type );

/**************************************************************************************************/
/**@brief  This fuction is used to pass packet to network layer.
 
 * @param in nwkmsg* ; As input it require a pointer to struct typed of nwkmsg
 
 * @param out success;  This function will pass structure to network layer and it will send 
                       message to particular client/server
 
 * @return  function return 1 after successfully sending of message;
*/
int APDE_NWK( NwkMsg_t * msg);
/**************************************************************************************************/

/** @brief  This fuction is used to call back when some message is recived by reciving thread
 
 * @param in (*MCPS_ZCL_SapHandler)(nwkmsg) ; function pointer which you want to call
 * @param out rear;function will start rquired thread and initialize globle variable 
 * @return  It will invoke respected finction when some message arrived
*/
void NW_RegisterSapHandlers(void ( *MCPS_ZCL_SapHandler )( NwkMsg_t ), void ( *DeviceRemove )( uint16_t ID ) );
/**************************************************************************************************/
                                

#endif /* nwk */
