/*****************************************************************************
 *Copyright(c) 2017, Volansys Technologies
 * 
 * Description:
 * @file zdo.c 
 * @brief: Defination & decleration for ZigBee Device Object. 
 *    
 * Author       - Reshma Patel
 *
 ***********************************************************************************
 *
 * History
 *                      
 * March/23/2017, Reshma Patel, Created 
 *
 ***********************************************************************************/

/***********************************************************************************
 * Include
 ************************************************************************************/
#include <inttypes.h>
#include "NWK.h"
#include "APS.h"
#include "zdo.h"

/***********************************************************************************
 * Global variables
 ************************************************************************************/
int position = 0;
int term = 1;
/************************************************************************************************
 * Function prototype
 ************************************************************************************************/
void simpleDescReq(uint16_t add);
void maintainTable(uint16_t shortAddr, uint16_t clusterId);
void MCPS_ZCL_SapHandler( NwkMsg_t msg );
void *cordHandler(void *arg);
void ZDO_Start(int node);
deviceList *getDevice();
void removeDevice(uint16_t remId);


void simpleDescRsp(uint16_t addr)
{
    struct DATA_Request *dataRsp;
   
    dataRsp = malloc(sizeof(struct descResponse));
    frameRsp = malloc(sizeof(struct descResponse));   
    frameRsp->u16MsgType = MSG_SIMPLE_DESCRIPTOR_RESPONSE;
    frameRsp->u16Length = sizeof(struct descRsp);
    frameRsp->payLoad.shortAddr = 0X01;
    frameRsp->payLoad.clusterId = ZCL_CLUSTERID_ONOFF;
    
    //PRINTD("message type in descriptor response %04X\n", frameRsp->u16MsgType); 
    
    dataRsp->ADSULength = sizeof(struct descResponse);
    dataRsp->DstAddress = addr;
    dataRsp->DstEndpoint = 0x00;
    memcpy((void *)dataRsp->ADSU, (void *)frameRsp, dataRsp->ADSULength);
    
    PRINTD("Simple Descriptor response send\n");
    
    APSDE_DATA_Request( dataRsp );
}

/************************************************************************************************
 * @brief simple descriptor request function
 * @param[in] add device id.
 * @return void
 ************************************************************************************************/
void simpleDescReq(uint16_t add)
{
    struct DATA_Request data;
    
    PRINTD("\n IN simple descriptor request \n\n");
    //PRINTD("\n\nDevice id in sdr:%"PRIu16"\n\n",add);
    
    if((frameD = malloc(sizeof(struct descriptorReq))) == NULL)
    {
        printf("memory allocation failure\n");
    }
    
    else
    {
        frameD->u16MsgType = MSG_SIMPLE_DESCRIPTOR_REQUEST;
        frameD->u16Length = sizeof(struct _SimpleDescriptor);
        frameD->payLoad.u16TargetAddress = add;
        frameD->payLoad.u8Endpoint = 0;
        
        data.ADSULength = frameD->u16Length;
          
        memcpy((void*)data.ADSU, (void*)frameD, frameD->u16Length);
        data.DstEndpoint = 0x00;		
        data.DstAddress = add;
        PRINTD("data send\n");
        
        APSDE_DATA_Request( &data );
    }
}

/************************************************************************************************
 * @brief display device List
 * @param[in] structure pointer
 * @return void
 ************************************************************************************************/
void display(deviceList* head)
{
    deviceList* ptr;
    ptr = head;

    do {
        printf("device Id : %d\t", ptr->deviceId);
        printf("cluster Id : %d\t", ptr->clusterId);
        printf("device Type : %s\n", ptr->deviceType);
        ptr = ptr->next;
    } while(ptr != NULL); 
}

/************************************************************************************************
 * @brief whenever new device announce this handler will update table 
 available to Application layer.
 * @param[in] shortAddr device short address
 * @param[in] clusterId cluster id of that device
 * @return void
 ************************************************************************************************/
void maintainTable(uint16_t shortAddr, uint16_t clusterId)
{
    int i;
    deviceList *new, *temp, *temp1;

    if( (new = (deviceList *)malloc(sizeof(deviceList))) == NULL)
    {
        printf("memory allocation failure\n");
    }
    else 
    {
        new->deviceId = shortAddr;
        new->clusterId = clusterId;
        if(clusterId == 6)
        {
        strcpy(new->deviceType, "smart_bulb");
        }
        
        new->next = NULL;

        if(head == NULL) {
            head = new;
        }
        temp = head;
        for(i = 0; i < position-1; i++) {
            temp = temp->next;
        }
        temp1 = temp->next;
        temp->next = new;
        new->next = temp1;
        position++;
    }
    PRINTD("Device List Updated\n");
    //display(head);
	term = 0;
}

int findEntry(uint16_t id)
{
    deviceList *ptr;
    ptr = head;
    while(ptr != NULL)
    {
        if(ptr->deviceId == id)
        {
            PRINTD("device Id matched with previous entry\n");
            return 0;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    return 1;
}


/************************************************************************************************
 * @brief takes message from Application layer
 * @param[in] NwkMsg_t message from network
 * @return void
 ************************************************************************************************/
void APME_ZDO_Saphandler( DataIndication_t IndicationData )
{
    uint16_t cluster_Id;

    uint16_t devId = IndicationData.DstAddress;
    uint8_t len = IndicationData.ADSULength; 
    void *packet = IndicationData.ADSU;
    uint16_t type = *(uint16_t *)( packet+ MESSAGE_TYPE );
    PRINTD("Message type Field %04x\n",type);

    switch(type)
    {
        case MSG_DEVICE_ANNOUNCE:
            PRINTD("device Announce received\n");
            simpleDescReq( devId );
            break;

        case MSG_SIMPLE_DESCRIPTOR_RESPONSE:
            PRINTD("Device simple descriptor response received\n");
            cluster_Id = *(uint16_t*)(packet + 6);
            if(findEntry(devId) == 1)
            {
                //PRINTD("cluster Id :%"PRIu16"\n", cluster_Id);
                maintainTable(devId, cluster_Id);
            }
            break;
        default:
            printf("invalid message type\n");
            break;
    }
}



void APME_Deviceremove_Saphandler( uint16_t device_ID )
{
	removeDevice(device_ID);
}	

void removeDevice(uint16_t remId)
{
    
    deviceList  *ptr, *tmp = NULL;
    ptr = head;

    while ( ptr != NULL )
    {
        if( ptr->deviceId  == remId )
        {
            position--;
            if(tmp==NULL)
            {
                head = head->next;
            }
            else
            {
                tmp->next  = ptr->next;
            }

            free( ptr );    
            break;
        }
        tmp = ptr;
        ptr = ptr->next;
    }       
}

/************************************************************************************************
 * @brief cordinator thread
 * @param[in] void*
 * @return void
 ************************************************************************************************/
void *cordHandler(void *arg)
{
    PRINTD("cordinator ZDO starts\n");
}

/************************************************************************************************
 * @brief ZDO initialization
 * @param[in] node init as coordinator
 * @return void
 ************************************************************************************************/
void ZDO_Start(int node)
{

    pthread_t cordTid, endTid;
    switch(node)
    {
        case 1: 
            pthread_create( &cordTid, NULL, &cordHandler, NULL);
            pthread_join( cordTid, NULL);
            break;

        default:
            printf("enter valid number\n");
            break;
    }
	while(term == 1);
}

/************************************************************************************************
 * @brief return pointer to structure deviceList whenever application ask for
 * @param[in] void
 * @return pointer to structure
 ************************************************************************************************/
deviceList *getDevice()
{
    return head;
}

