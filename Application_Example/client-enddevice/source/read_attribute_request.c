/*******************
 * Includes
 *******************/
#include "read_attribute_request.h"

/*************************
 * Defines
 *************************/


/***********************
 * Global Variables
 ***********************/
/* None */

/************************
 * Function Prototypes
 ************************/
int readAttributeRequest ( struct command *_cmd, void **_packet, int *_len )
{
	PRINTD("******Checking inputs of read req.******\n");						/* for Debugging */
	PRINTD("cluster id is %04x\n", _cmd -> clusterId);						/* for Debugging */
	if ( validateClusterId ( _cmd -> clusterId, _cmd -> attributeId ) != VALID_DATA ) {
		printf("attribute id or cluster id invalid\n");						/* if given cluster id doesnt exist or the 
													   attribute id doesnt exist the prints error */
		return UNSUCCESSFUL;
	}
	PRINTD("all inputs good!!!\n");
	PRINTD("~~~~~~~~~~~making the packet for read request~~~~~~~~\n");/* for Debugging */
	typedef struct {
		uint8_t	u8FrameControl;									/* 8bit frame control */
		//uint16_t u16ManufacturerCode;								//omitted as no manufacturer exist for now
		uint8_t u8TransmissionSequenceNumber;							/* 8bit transmission number */
		uint8_t u8CommandIdentifier;								/* 8bit command id */
	} __attribute__((__packed__)) frame_header_t;

	void *readPacket = malloc( sizeof( frame_header_t ) + sizeof( uint16_t )*2 );
	/* malloc for the end packet */

	int indice = 0;

	*( uint8_t * )( readPacket + indice ) = FRAME_TYPE_ENTIRE_PROFILE | EXCLUDE_MANUFACTURER_SPECIFIC | SERVER_TO_CLIENT | DISABLE_DEFAULT_RESPONSE;
	/* setting flags in frame control byte */
	indice++;
	//	readPacket->u16ManufacturerCode= KEEP YOUR MANUFACTURER CODE;
	/*	( uint16_t * )( readPacket + indice ) = MANUFACTURER_CODE;
		indice += 2;
		*/
	*( uint8_t * )( readPacket + indice ) = 00;				/* tx no. */
	indice++;
	*( uint8_t * )( readPacket + indice ) = _cmd -> commandId;		/* comm id */
	indice++;
	*(uint16_t *)(readPacket+indice ) = _cmd -> attributeId;		/* attribute id */
	indice += 2;

	*_len = indice;								/* copying the packet length */
	/****************** to verify the packet enable this set of code *******************/
	/*
	   indice=0;
	   printf("u8FrameControl %d\n",*( uint8_t * )( readPacket + indice ));
	   indice++;
	   printf("u16ManufacturerCode  %d\n",*(uint16_t *)( readPacket + indice ));
	   indice+=2;
	   printf("u8TransmissionSequenceNumber %d\n",*( uint8_t * )( readPacket + indice ));
	   indice++;
	   printf("u8CommandIdentifier %04x\n",*( uint8_t * )( readPacket + indice ));
	   indice++;
	   printf("attribute id %04x\n",*(uint16_t *)(readPacket + indice ));
	   indice+=2;
	   */
	*_packet = readPacket;
	PRINTD("******** Packet forming successful *********\n\n");
	return SUCCESSFUL;
}


int readAttributeRequestDecoder ( struct command *_cmd, void **_packet, int *_len )
{
	PRINTD("************* Read Request decoder *************\n");	/* for Debugging */

	uint8_t frameControl;										/* will hold the frame control byte of incoming packet */
	uint8_t transmissionSeqNumber;
	uint16_t manufacturerCode;
	int indice = 0;
	bool manufacturerSpecificFlag = 0;								/* flag for manufacturer code */
	frameControl = *( uint8_t * )( *_packet + indice );
	indice++;
	/************** forviewing the frame control byte **************/
	PRINTD("recieved frameControl :%d\n",frameControl);
	if ( ( frameControl & FRAME_TYPE_CLUSTER_SPECIFIC ) == 1 ) {
		_cmd -> clusterSpecific = 1;
		PRINTD("Cluster specific command\n");
	} else {
		_cmd -> clusterSpecific = 0;
		PRINTD("Command across entire profile\n");
	}
	if ( ( frameControl & INCLUDE_MANUFACTURER_SPECIFIC ) == INCLUDE_MANUFACTURER_SPECIFIC ) {
		PRINTD("Manufacturer code present\n");
		manufacturerSpecificFlag = 1;
	} else {
		PRINTD("Manufacturer code not included\n");
	}
	if ( ( frameControl & SERVER_TO_CLIENT ) == SERVER_TO_CLIENT ) {
		PRINTD("packet from server\n");
	} else {
		PRINTD("packet from client\n");
	}
	if ( ( frameControl & DISABLE_DEFAULT_RESPONSE ) == DISABLE_DEFAULT_RESPONSE ) {
		PRINTD("Disable default response\n");
	} else {
		PRINTD("Enable default response\n");
	}
	if ( manufacturerSpecificFlag == 1 ) {								/* if manufacturer flag set than read manufacturer code */
		manufacturerCode = *( uint16_t * )( *( _packet ) + indice );
		indice += 2;
		PRINTD("manufacturer id %d\n",manufacturerCode);
	}

	transmissionSeqNumber = *( uint8_t * )( *( _packet ) + indice );/* read transmission number */
	indice++;
	PRINTD("transmissionSeqNumber %d\n",transmissionSeqNumber);	
	_cmd -> commandId = *( uint8_t * )( *( _packet ) + indice );	/* reads the command id */
	PRINTD("commandId %d\n", _cmd -> commandId);
	indice++;
	_cmd -> attributeId = *( uint16_t * )( *( _packet ) + indice );	/* read the attribute id */
	PRINTD("attribute requested %#04x\n\n",_cmd -> attributeId);
	return SUCCESSFUL;
}
