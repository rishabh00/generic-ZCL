/*****************************************************************************
 * Description:
 * @file APS.h 
 * @brief: Defination & decleration for network management 
 *    
 * Author       - Daksha Vasani
 *
 *******************************************************************************
 *
 * History
 *                      
 * March/23/2017, Daksha, Created 
 *
 ******************************************************************************/

#ifndef __APS__
#define __APS__


/*********************************************************************************
 * Includes
 ********************************************************************************/
#include <NWK.h>
#include "global.h"

/**********************************************************************************
 * Defines
 **********************************************************************************/
#define PACKED __attribute__((__packed__))
/******************************************************************************
 * Enumeration
 ******************************************************************************/
typedef enum
{
    SUCCESS,
    NO_SORT_ADDRESS,
    SECURITY_FAIL,
    NO_ACK,
    ADSU_TOO_LONG,
}StatusConfirm_e; 

/**********************************************************************************
 * structures
 **********************************************************************************/

typedef union {
    struct {
        char FrameType:2;
        char DeliveryMode:2;
        char AckFormat:1;
        char Security:1;    
        char AckRequest:1;
        char ExtendedHeaderPresent:1;
    };
    uint8_t raw;
}PACKED FrameControl_t;

typedef struct APS_Frame{
    FrameControl_t ControlField;
    uint8_t u8DestinationEndPoint;
    uint16_t u16GroupAddress;
    uint16_t u16ClusterIdentifier;
    uint16_t u16ProfileIdentifier;
    uint8_t u8SourceEndPoint;
    uint8_t u8APSCounter;
    uint8_t packet[100];
}PACKED frame_t;

struct DATA_Request
{
    uint8_t DstAddrMode;
    uint8_t DstAddress;	
    uint8_t DstEndpoint;
    uint16_t ProfileId;
    uint16_t ClusterId;
    uint8_t SrcEndpoint;
    uint8_t ADSULength;
    uint8_t ADSU[100];
    uint8_t TxOptions;
    uint8_t RadiusCounter;
}PACKED ;

typedef struct
{
    uint8_t DstAddrMode;
    uint8_t DstAddress;	
    uint8_t DstEndpoint;
    uint8_t SrcEndpoint;
    uint8_t SrcAddress;	
    uint16_t ProfileId;
    uint16_t ClusterId;
    uint8_t ADSULength;
    uint8_t ADSU[100];
    StatusConfirm_e Status;    
    uint8_t LinkQuality;
    uint8_t RxTime;
}PACKED DataIndication_t;

/************************************************************************************************
 * Function prototype
 ************************************************************************************************/
/************************************************************************************************
 * @brief callback function of APS.Network layer will call this function when it will receive packet 
 * @param msg
 * @return void
 ************************************************************************************************/
void MCPS_ZCL_SapHandler( NwkMsg_t msg );
 
/**************************************************************************************************
/**************************************************************************************************
 * @brief this function will be used by upper layer to send data request
 * @param *req
 * @return void
 **************************************************************************************************/
void APSDE_DATA_Request( struct DATA_Request *req );

/***************************************************************************************************
 * @brief APS intialization. function will register the callback function of network layer
 * @param void
 * @return void
 ***************************************************************************************************/
void APS_init();

/***************************************************************************************************
 * @brief APS callback function register handler.this function will be used by upper layer to register 
		  callback function of it.
 * @param function pointer of ZDO and ZCL
 * @return void
 ***************************************************************************************************/
void APS_RegisterSapHandlers(void (*APME_ZDO_SapHandler)( DataIndication_t MsgDecode ), void (*APDE_ZCL_SapHandler)( DataIndication_t MsgDecode ), void (*DeviceRemove)( uint16_t ID ) );

/***************************************************************************************************/
void DeviceRemove( uint16_t device_ID );
#endif /* APS */

