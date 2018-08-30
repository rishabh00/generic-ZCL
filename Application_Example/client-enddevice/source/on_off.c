/*******************
 * Includes
 *******************/
#include "on_off.h"

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
void onoffInit()
{
	onoff_t.onOff = 0x00;					/* intialize the value of onoff variable */
}

int validateOnOffAttr ( uint16_t _arr )
{
	PRINTD("validating attributes\n");
	switch ( _arr ) {					/* validates the attributes */
		case ZCL_ATTRIBUTE_ID_ONOFF_ONOFF : 
			PRINTD("Valid attribute\n");
			break;
		default : 
			return INVALID_ATTRIBUTE;
	}
	return SUCCESSFUL;
}
