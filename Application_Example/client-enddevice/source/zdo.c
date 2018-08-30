/*****************************************************************************
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

#include "NWK.h"
#include "zdo.h"
#include "APS.h"

/***********************************************************************************
 * Global variables
 ************************************************************************************/
int position = 0;
int term = 1;
/*************************************************************************************
 * Function prototype
 *************************************************************************************/
void simpleDescRsp(uint16_t addr);
void deviceAnnce();
void *clientThread(void *arg);
void simpleDescReq(uint16_t add);
void ZDO_Start(int node);

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
		printf("device id : %d\t", ptr->deviceId);
		printf("shortAddr : %d\n", ptr->clusterId);
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
	printf("table\n");

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
		new->next = NULL;

		if(head == NULL) {
			head = new;
			//	position++;
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
	printf("connected device List\n");
	display(head);

}

int findEntry(uint16_t id)
{
	deviceList *ptr;
	ptr = head;
	while(ptr != NULL)
	{
		if(ptr->deviceId == id)
		{
			printf("id matched with previous entry\n");
			return 0;
		}
		else
		{
			ptr = ptr->next;
		}
	}
	return 1;
}


/**************************************************************************************
 * @brief SimpleDescRsp form response frame 
 * @param[in] addr device Id of end node
 * return void
 **************************************************************************************/
void simpleDescRsp(uint16_t addr)
{
	struct DATA_Request *dataRsp;
	dataRsp = malloc(sizeof(struct descResponse));
	frameRsp = malloc(sizeof(struct descResponse));   
	frameRsp->u16MsgType = MSG_SIMPLE_DESCRIPTOR_RESPONSE;
	frameRsp->u16Length = sizeof(struct descRsp);
	frameRsp->payLoad.shortAddr = 0X01;
	frameRsp->payLoad.clusterId = ZCL_CLUSTERID_ONOFF;
	PRINTD("message type in descriptor response %04X\n", frameRsp->u16MsgType); 
	dataRsp->ADSULength = sizeof(struct descResponse);
	memcpy((void *)dataRsp->ADSU, (void *)frameRsp, dataRsp->ADSULength);
	dataRsp->DstAddress = addr;
	dataRsp->DstEndpoint = 0x00;
	APSDE_DATA_Request( dataRsp );
	term = 0;
}

/**************************************************************************************
 * @brief deviceAnnce form frame from which coordinator ZDO found that 
 this device want to join in
 * @param[in] addr device Id of end node
 * return void
 **************************************************************************************/
void deviceAnnce()
{
	PRINTD("\n*************** IN DEVICE ANNCE ************\n\n");
	int size;
	newFrame *frame;
	struct DATA_Request data; 
	frame = malloc(sizeof(struct dataZDA));
	size = sizeof(struct annce);
	frame->u16Type = MSG_DEVICE_ANNOUNCE;
	frame->u16Length = size;
	frame->payLoad.devId = 0xffff;
	frame->payLoad.iEEEAdd = 0xffffffffffff;
	frame->payLoad.capability = 0x71;
	data.ADSULength = sizeof(struct dataZDA);
	memcpy( (void*)data.ADSU, (void*)frame, data.ADSULength);
	data.DstEndpoint = 0x00;
	PRINTD("\nframe typee%"PRIu16"\n", frame->u16Type);
	PRINTD("announce send\n");
	APSDE_DATA_Request( &data );
}

/**************************************************************************************
 * @brief client thread routine provide functionality whether
 end device wants to announce it self or not
 * @param[in] void*
 * return void
 **************************************************************************************/

void *clientThread(void *arg)
{
	char ch;
	do{
		printf("enter y/Y to announce the device : ");
		scanf(" %c", &ch);
		if(ch == 'y' || ch == 'Y' ){
			deviceAnnce();
			PRINTD("return from ancce\n");
			break;
		}
		else
			printf("device not announced\n");
	}while(1);

}

/**************************************************************************************
 * @brief simpleDescReq request for simple descriptor
 * @param[in] add simple descriptor for that specified end node
 * return void
 **************************************************************************************/
void simpleDescReq(uint16_t add)
{
	struct DATA_Request data;

	if((frameD = malloc(sizeof(struct descriptorReq))) == NULL)
	{
		printf("memory allocation failure\n");
	}
	else
	{
		PRINTD("client forming req for simple descriptor\n");
		frameD->u16MsgType = MSG_SIMPLE_DESCRIPTOR_REQUEST;
		frameD->u16Length = sizeof(struct _SimpleDescriptor);
		frameD->payLoad.u16TargetAddress = add;
		frameD->payLoad.u8Endpoint = 0;
		data.ADSULength = frameD->u16Length;
		memcpy((void*)data.ADSU, (void*)frameD, frameD->u16Length);
		data.DstEndpoint = 0x00;		
		data.DstAddress = add;
		APSDE_DATA_Request( &data );
	}
}

/************************************************************************************************
 * @brief takes message from network layer
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

		case MSG_SIMPLE_DESCRIPTOR_REQUEST:
			simpleDescRsp( devId );
			break;

		default:
			printf("invalid message type\n");
			break;
	}
}


/************************************************************************************************
 * @brief ZDO initialization
 * @param[in] node init as end node
 * @return void
 ************************************************************************************************/
void ZDO_Start(int node)
{
	pthread_t endTid;
	switch(node)
	{
		case 2:
			pthread_create( &endTid, NULL, &clientThread, NULL);
			pthread_join( endTid, NULL);
			break;
		default:
			printf("enter valid number\n");
			term = 0;
			break;
	}
	while(term == 1);
}
