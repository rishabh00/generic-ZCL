/*******************
 * Includes
 *******************/
#include "zcl.h"

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
int zclEncoder ( struct command *_cmd, void **_packet, int *_len )
{
	PRINTD("~~~~~~~~ IN ZCL ENCODER ~~~~~~~\n\n");				/* for Debugging */
	if ( _cmd -> clusterSpecific == 1 ) {					/* checks whether the packet has cluster specific command */
		makeClusterSpecificPacket (_cmd, _packet, _len);
	} else {								/* if not then executes general command */
		switch ( _cmd -> commandId ) {
			case READ_ATTRIBUTE_ID : 
				readAttributeRequest ( _cmd, _packet, _len ); 
				break;
			case READ_ATTRIBUTE_RESPONSE_ID : 
				readAttributeResponse ( _cmd, _packet, _len ); 
				break;
			/*case WRITE_ATTRIBUTE_ID	:			// left for future implementation 
			case WRITE_ATTRIBUTE_UNDIVIDED_ID :
			case WRITE_ATTRIBUTE_RESPONSE_ID	:
			case WRITE_ATTRIBUTE_NO_RESPONSE_ID :
			case CONFIGURE_REPORTING_ID :
			case CONFIGURE_REPORTING_RESPONSE_ID :
			case READ_REPORTING_CONFIGURATION_ID :
			case READ_REPORTING_CONFIGURATION_RESPONSE_ID :
			case REPORT_ATTRIBUTES_ID :
			case DEFAULT_RESPONSE_ID :
			case DISCOVER_ATTRIBUTE_ID :
			case DISCOVER_ATTRIBUTE_RESPONSE_ID :
			case READ_ATTRIBUTE_STRUCTURED_ID :
			case WRITE_ATTRIBUTE_STRUCTURED_ID :
			case WRITE_ATTRIBUTE_STRUCTURED_RESPONSE_ID :
			*/			
			default : 
				printf("Invalid Command ID\n");
				break;
		}
	}
	return SUCCESSFUL;
}


int zclDecoder ( struct command *_cmd, void **_packet, int *_len )
{
	PRINTD("\n~~~~~~~~ IN ZCL DECODER ~~~~~~~\n\n");
	if ( *( uint8_t* )( *_packet ) & FRAME_TYPE_CLUSTER_SPECIFIC ) {		/* checks whether the packet has cluster specific command */
		decodeClusterSpecificPacket(_cmd,_packet,_len);
		PRINTD("_cmd->command id %d\n",_cmd->commandId);
		return SUCCESSFUL;
	} 
	else 
	{
		/* if not then executes general command */
		switch (*(uint8_t *)(*_packet + 2)){
			case READ_ATTRIBUTE_ID :  
				readAttributeRequestDecoder(_cmd,_packet,_len); 	/* decode read attribute request */
				break;
			case READ_ATTRIBUTE_RESPONSE_ID : 
				readAttributeResponseDecoder(_cmd,_packet,_len); 	/* decode read attribute request response */
				break;
				// left for future implementation
				/*case WRITE_ATTRIBUTE_ID :										
				  case WRITE_ATTRIBUTE_UNDIVIDED_ID :
				  case WRITE_ATTRIBUTE_RESPONSE_ID	:
				  case WRITE_ATTRIBUTE_NO_RESPONSE_ID :
				  case CONFIGURE_REPORTING_ID :
				  case CONFIGURE_REPORTING_RESPONSE_ID :
				  case READ_REPORTING_CONFIGURATION_ID :
				  case READ_REPORTING_CONFIGURATION_RESPONSE_ID :
				  case REPORT_ATTRIBUTES_ID :
				  case DEFAULT_RESPONSE_ID :
				  case DISCOVER_ATTRIBUTE_ID :
				  case DISCOVER_ATTRIBUTE_RESPONSE_ID :
				  case READ_ATTRIBUTE_STRUCTURED_ID :
				  case WRITE_ATTRIBUTE_STRUCTURED_ID :
				  case WRITE_ATTRIBUTE_STRUCTURED_RESPONSE_ID :
				  */

			default : 
				printf("invalid command id\n");
				break;
		}
	}
	return SUCCESSFUL;
}

