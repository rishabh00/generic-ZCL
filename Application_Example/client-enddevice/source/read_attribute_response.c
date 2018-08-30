/*******************
 * Includes
 *******************/
#include "read_attribute_response.h"

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
int readAttributeResponse ( struct command *_cmd, void **_packet, int *_len )
{
	PRINTD("******* Making packet for request response ********\n");
	typedef struct {
		uint8_t	u8FrameControl;							/* 8bit frame control */
		//uint16_t u16ManufacturerCode;						//omitted as no manufacturer exist for now
		uint8_t u8TransmissionSequenceNumber;					/* 8bit transmission number */
		uint8_t u8CommandIdentifier;						/* 8bit command id */
	} __attribute__((__packed__)) frame_header_t;
		
	void *readPacket = malloc( sizeof( frame_header_t ) + sizeof( uint16_t )*2 );	/* malloc for the end packet */
	int indice=0;

	*( uint8_t* )( readPacket + indice ) = FRAME_TYPE_ENTIRE_PROFILE | EXCLUDE_MANUFACTURER_SPECIFIC | CLIENT_TO_SERVER | DISABLE_DEFAULT_RESPONSE ;
																	/* setting flags in frame control byte */
	indice++;
	*( uint8_t* )( readPacket + indice ) = 0;				/* pushing the transimission number */
	indice++;
	*( uint8_t* )( readPacket + indice ) = _cmd -> commandId;		/* pushing the command id */
	indice++;
	*( uint16_t* )( readPacket + indice ) = _cmd -> attributeId;		/* pushing the attribute id */
	indice += 2;
	*( uint16_t* )( readPacket + indice ) = _cmd -> data;			/* pushing the data */
	indice += 2;

	*_packet = readPacket;
	*_len = indice;

	return SUCCESSFUL;
}

int readAttributeResponseDecoder(struct command *_cmd, void **_packet,int *_len)
{
	PRINTD("************ RESPONSE DECODER ***************\n");		/* for Debugging */
	PRINTD("commencing read attribute response decoder\n\n");		/* for Debugging */
	_cmd -> clusterSpecific = 0;						/* filling structure */
	_cmd -> commandId = READ_ATTRIBUTE_RESPONSE_ID;
	_cmd -> attributeId = *( uint16_t* )( *_packet + 3 );
	_cmd -> data = *( uint16_t* )( *_packet + 5 );
	return SUCCESSFUL;
}
