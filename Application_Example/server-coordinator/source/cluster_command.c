/*******************
 * Includes
 *******************/
#include "cluster_command.h"

/*************************
 * Defines
 *************************/

/***********************
 * Global Variables
 ***********************/
/* None */

/*****************************************************************************
 * Function Prototypes
 ****************************************************************************/
int makeClusterSpecificPacket ( struct command *_cmd, void **_packet, int *_length )
{
	PRINTD("making cluster specific command\n");		/* for Debugging */
	typedef struct {
		uint8_t	u8FrameControl;							/* frame control byte of frame header */
//		uint16_t u16ManufacturerCode;					/* omitted as no manufacturer exist for now */
		uint8_t u8TransmissionSequenceNumber;			/* transmission sequence number */
		uint8_t u8CommandIdentifier;					/* command id */
	} __attribute__((__packed__))  frame_header_t;

	frame_header_t *header;
	
	header = malloc ( sizeof ( frame_header_t ) );

	header -> u8FrameControl = FRAME_TYPE_CLUSTER_SPECIFIC | EXCLUDE_MANUFACTURER_SPECIFIC | SERVER_TO_CLIENT | DISABLE_DEFAULT_RESPONSE ;
	
	header -> u8TransmissionSequenceNumber = 0;
	
	header -> u8CommandIdentifier = _cmd->commandId;

	*_length = sizeof ( frame_header_t ) / sizeof ( uint8_t );
	
	*_packet = header;	
	
	return SUCCESSFUL;
}

int decodeClusterSpecificPacket ( struct command *_cmd, void **_packet, int *_length )
{
		PRINTD("decoding cluster specific command\n");		/* for Debugging */
	
		PRINTD("cluster id =  %#04x\n",_cmd->clusterId);	/* for Debugging */

		if ( _cmd -> clusterId == ZCL_CLUSTERID_ONOFF ) {	/* prints the cluster specific command */
			PRINTD("command is specific to ONOFF cluster and the command is ");
		
			if ( *( uint8_t * )( *_packet + 2 ) == ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_OFF )
				PRINTD("off\n");
		
			else if ( *( uint8_t * )( *_packet + 2 ) == ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_ON )
				PRINTD("on\n");
		
			else if ( *( uint8_t * )( *_packet + 2 ) == ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_TOGGLE )
				PRINTD("toggle\n");
				
		}
    _cmd->clusterSpecific=1;
	_cmd->commandId = *( uint8_t * )( *_packet + 2 );	/* saves the cluster specific command into the pointer */
	
	return SUCCESSFUL;
}
