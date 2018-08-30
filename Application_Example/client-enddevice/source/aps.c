/******************************************************************************
 * Description: Unit test of Application support sub layerserver part
 ******************************************************************************
 * History
 *
 * 22/3/2017 - Daksha Vasani 
 ******************************************************************************/
/******************************************************************************
 *	Includes
 ******************************************************************************/
#include "NWK.h"
#include "APS.h"
/*****************************************************************************
 * 	Defines,structures
 *****************************************************************************/
#define PACKED __attribute__((__packed__))
#define QUEUESIZE 			512
#define APSKEY 				1234
#define FRAME_TYPE 			( 0 )
#define DESTINATION_ENDPOINT 		( 0 + 1 )
#define GROUP_ADDRESS 			( 0 + 1 + 1 )
#define CLUSTER_IDENTIFIER 		( 0 + 1 + 1 + 2 )
#define PROFILE_IDENTIFIER 		( 0 + 1 + 1 + 2 + 2 )
#define SOURCE_ENDPOINT 		( 0 + 1 + 1 + 2 + 2 + 2 )
#define APS_COUNTER 			( 0 + 1 + 1 + 2 + 2 + 2 + 1 )
#define PACKET 				( 0 + 1 + 1 + 2 + 2 + 2 + 1 + 1 )
#define RADIUS 				( 0 + 1 + 1 + 2 + 2 + 2 + 1 + 1 + 2 )

#define	FRAME_TYPE_DATA 		0b00000000
#define	FRAME_TYPE_COMMAND 		0b00000001
#define	FRAME_TYPE_ACK 			0b00000010
#define	DELIVERY_MODE_UNICAST 		0b00000000
#define	DELIVERY_MODE_INDIRECT 		0b00000100
#define	DELIVERY_MODE_BROADCAST 	0b00001000
#define	DELIVERY_MODE_GROUP 		0b00001100
#define	ACK_FORMAT_FIELD_DISABLE	0b00000000
#define	ACK_FORMAT_FIELD_ENABLE		0b00010000
#define	SECURITY_SUB_FIELD_DISABLE	0b00000000
#define	SECURITY_SUB_FIELD_ENABLE	0b00100000
#define	ACK_REQUEST_SUB_FIELD_DISABLE	0b00000000
#define	ACK_REQUEST_SUB_FIELD_ENABLE	0b01000000
#define	EXTENDED_HEADER_PRESENT		0b10000000
#define	EXTENDED_HEADER_ABSENT		0b00000000

/*********************************************************************************************
 * Global variables
 *********************************************************************************************/
frame_t *APSPacket;

/************************************************************************************************
 * Function prototype
 ************************************************************************************************/
void ( *APME_ZDO_Handler )(DataIndication_t );
void ( *APDE_ZCL_Handler )( DataIndication_t );
void die(char *s);
void *RecvFromQueue( void *data );
void *frame_decoder( void *packet );
void APSDE_DATA_Indication( frame_t MsgDecode, int len,uint16_t devId );

/****************************************************************************************************
 * Function definition
 *****************************************************************************************************/

/********** Register callback functions of ZDO and ZCL ************/
void APS_RegisterSapHandlers(void (*APME_ZDO_SapHandler)( DataIndication_t MsgDecode ), void (*APDE_ZCL_SapHandler)( DataIndication_t MsgDecode ))
{
	/* APS Handler */
	APME_ZDO_Handler = APME_ZDO_SapHandler;
	APDE_ZCL_Handler = APDE_ZCL_SapHandler;
} 

/*********** Callback function of APS  *************/
void MCPS_ZCL_SapHandler( NwkMsg_t msg )
{
	int ret;
	NwkMsg_t *msg1;
	if ( ( msg1 = malloc( sizeof( NwkMsg_t ) ) ) == NULL )
	{
		printf("memory allocation failure\n");
		exit( EXIT_FAILURE );
	}
	memcpy( msg1, &msg, sizeof( NwkMsg_t ) );
	pthread_t thrd;
	ret = pthread_create( &thrd, NULL, &frame_decoder, msg1 );
	if ( ret != 0 )
	{
		printf("Erorr in creating thread\n");
	}
}

/********** Initialization of Application support sub layer ***********/
void APS_init()
{
	NW_RegisterSapHandlers( &MCPS_ZCL_SapHandler);
}

/************ Frame decoder ************/
void *frame_decoder( void *msg )
{
	NwkMsg_t *tmp = ( NwkMsg_t * )msg;
	void *packet = tmp->packet;
	uint16_t len;
	uint16_t devId;
	devId = tmp->device_id;
	len = ( ( tmp->packet_length )  - PACKET );
	frame_t MsgDecode;
	MsgDecode.ControlField.raw = *(uint8_t *)( packet+ FRAME_TYPE );
	PRINTD("Aps frame Recieved......trying to decode it\n");
	if( MsgDecode.ControlField.FrameType & 0b10 )
		PRINTD("Acknoledgement frame\n");
	else if( MsgDecode.ControlField.FrameType & 0b01 )
		PRINTD("Command frame\n");
	if( MsgDecode.ControlField.FrameType & 0b00 )
		PRINTD("Data frame\n");
	if( MsgDecode.ControlField.DeliveryMode & 0b11 )
		PRINTD("Group delivery\n");
	else if( MsgDecode.ControlField.DeliveryMode & 0b10 )
		PRINTD("Broadcast delivery\n");
	else if( MsgDecode.ControlField.DeliveryMode & 0b01 )
		PRINTD("Indirect delivery\n");
	else if( MsgDecode.ControlField.DeliveryMode & 0b00 )
		PRINTD("Unicast delivery\n");
	if( MsgDecode.ControlField.AckFormat & 0b1 )
		PRINTD("Command frame acknoledgement\n");
	else
		PRINTD("Data frame acknoledgement\n");

	if( MsgDecode.ControlField.Security & 0b1 )
		PRINTD("Secure frame\n");
	else
		PRINTD("insecure frame\n");
	if( MsgDecode.ControlField.AckRequest & 0b1 )
		PRINTD("recipient have to send acknowledgement\n");
	else
		PRINTD("no need to send acknowledgement\n");

	if( MsgDecode.ControlField.ExtendedHeaderPresent & 0b1 )
		PRINTD("ExtendedHeader Present\n");
	else
		PRINTD("ExtendedHeader not Present\n");
	MsgDecode.u8DestinationEndPoint = *(uint8_t *)( packet + DESTINATION_ENDPOINT );
	MsgDecode.u16GroupAddress = *(uint16_t *)( packet + GROUP_ADDRESS );
	MsgDecode.u16ClusterIdentifier = *(uint16_t *)( packet +CLUSTER_IDENTIFIER );
	MsgDecode.u16ProfileIdentifier = *(uint16_t *)( packet + PROFILE_IDENTIFIER );
	MsgDecode.u8SourceEndPoint = *(uint8_t *)( packet + SOURCE_ENDPOINT );
	MsgDecode.u8APSCounter = *(uint8_t *)( packet + APS_COUNTER );
	memcpy( MsgDecode.packet, ( packet + PACKET ), len );
	APSDE_DATA_Indication( MsgDecode, len, devId );
}

/************* Filing structure for data indication to the upper layer ***************/
void APSDE_DATA_Indication( frame_t MsgDecode, int len, uint16_t devId )
{
	DataIndication_t IndicationData;
	IndicationData.DstAddrMode = 0x02;
	IndicationData.DstAddress  = devId;
	IndicationData.DstEndpoint = MsgDecode.u8DestinationEndPoint;
	IndicationData.SrcEndpoint = MsgDecode.u8SourceEndPoint;
	IndicationData.SrcAddress =  MsgDecode.u8SourceEndPoint;
	IndicationData.ProfileId = MsgDecode.u16ProfileIdentifier;
	IndicationData.ClusterId = MsgDecode.u16ClusterIdentifier;
	IndicationData.ADSULength = len;
	memcpy( IndicationData.ADSU,MsgDecode.packet,len);
	IndicationData.Status = SUCCESS;
	IndicationData.LinkQuality = 0xff;
	IndicationData.RxTime = 0x00;
	if ( MsgDecode.u8DestinationEndPoint == 0 )
		APME_ZDO_Handler( IndicationData );
	else
		APDE_ZCL_Handler( IndicationData );
}

/************* Function used by Application to send data request *************/
void APSDE_DATA_Request( struct DATA_Request *req )
{	
	NwkMsg_t *msg;
	if(( msg = malloc( sizeof( NwkMsg_t ) ) ) == NULL )
	{
		printf("memory allocation failure\n");
		exit( EXIT_FAILURE );
	}
	if(( APSPacket = malloc( sizeof( struct APS_Frame ) ) ) == NULL )
	{
		printf("memory allocation failure\n");
		exit( EXIT_FAILURE );
	}
	msg->device_id = req->DstAddress;
	APSPacket->ControlField.raw = FRAME_TYPE_COMMAND | DELIVERY_MODE_GROUP | ACK_FORMAT_FIELD_ENABLE | SECURITY_SUB_FIELD_ENABLE |ACK_REQUEST_SUB_FIELD_ENABLE | EXTENDED_HEADER_PRESENT;
	APSPacket->u8DestinationEndPoint = req->DstEndpoint;
	APSPacket->u16GroupAddress = 0x0000;
	APSPacket->u16ClusterIdentifier = req->ClusterId;
	APSPacket->u16ProfileIdentifier = req->ProfileId;
	APSPacket->u8SourceEndPoint = req->SrcEndpoint;
	APSPacket->u8APSCounter = req->RadiusCounter;
	memcpy(( void* )APSPacket->packet,( void* )req->ADSU, req->ADSULength);
	msg->packet_length = sizeof( struct APS_Frame );
	memcpy( ( void* )msg->packet, ( void* )APSPacket,msg->packet_length );

	if( APDE_NWK( msg ) == 0 ) {
		PRINTD("Message not sent\n");
	} else {
		PRINTD("Message successfully to destination\n");
	}

	free( msg );
	free( APSPacket ); 	
}

