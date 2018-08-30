/*****************************************************************************
 * Description:
 * @file zdo.h 
 * @brief: Defination & decleration for ZigBee Device Object
 *    
 * Author       - Reshma Patel
 *
 *******************************************************************************
 *
 * History
 *                      
 * March/23/2017, Reshma Patel, Created 
 *
 ******************************************************************************/

#ifndef ZDO_H
#define ZDO_H
#include "APS.h"
/**************************************************
 * Define 
 **************************************************/
#define PACKED __attribute__((__packed__))

#define MESSAGE_TYPE (0)
#define MESSAGE_LENGTH (0+1+1)

/**************************************************
 * Enumration 
 **************************************************/
//#define ZCL_CLUSTERID_ONOFF 0x0006
#include "cluster_id.h"

typedef enum {
    MSG_DEVICE_ANNOUNCE                =   0x004D,
    MSG_SIMPLE_DESCRIPTOR_REQUEST      =   0x0043,    
    MSG_SIMPLE_DESCRIPTOR_RESPONSE     =   0x8043,
} MsgType;

/****************************************************
 * Structure 
 ****************************************************/
typedef struct device
{
    uint16_t deviceId;
    uint16_t clusterId;
    char deviceType[20];
    struct device *next;
} deviceList;

struct annce                                                                
{                                                                           
    uint16_t devId;                                                         
    uint64_t iEEEAdd;                                                       
    uint8_t capability;                                                     
};                                                          

typedef struct dataZDA                                                              
{                                                                           
    uint16_t u16Type;                                                       
    uint16_t u16Length;                                                     
    struct annce payLoad;                                                   
} newFrame;

struct _SimpleDescriptor
{
    uint16_t    u16TargetAddress;
    uint8_t     u8Endpoint;
} sSimpleDescriptor;

struct descriptorReq
{
    uint16_t u16MsgType;
    uint16_t u16Length;
    struct _SimpleDescriptor payLoad;
} *frameD;

struct descRsp                                                                  
{                                                                               
    uint16_t shortAddr;                                                         
    uint16_t clusterId;                                                         
};                                                                              

struct descResponse                                                             
{                                                                               
    uint16_t u16MsgType;                                                        
    uint16_t u16Length;                                                         
    struct descRsp payLoad;                                                     
} *frameRsp;

/**************************************************************
 * Global variables
 *************************************************************/
newFrame *frame;

deviceList *head;

/*************************************************************
 * Function prototype
 ************************************************************/
/*************************************************************
 * @brief check message parsed by network and decode message 
 * 		  and procced further based on decoded output
 * @param[in] NwkMsg_t packet parsed by network layer
 * @return void
 ************************************************************/ 
void APME_ZDO_Saphandler( DataIndication_t IndicationData );

/*************************************************************
 * @brief  Initialize ZDO as coordinator or as end node
 * @param[in] node decide which one to Initialize
 * @return void
 ************************************************************/ 
void ZDO_Start(int node);

/*************************************************************
 * @brief  Provide service to Application layer to access 
		   device list
 * @param[in] void
 * @return pointer to structure
 ************************************************************/ 
deviceList *getDevice();
void APME_Deviceremove_Saphandler( uint16_t device_ID );

void display(deviceList* head);

#endif
