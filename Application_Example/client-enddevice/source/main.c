#include "myutility.h"
#include "APS.h"
#include "zcl.h"
#include "global.h"
#include "zdo.h"

int status = 0;

void MSGfromAPStoAPP( DataIndication_t  Msg );
void act(struct command *cmd);


int main()
{
	PRINTD("you are in the world of debug\n");
	int f=1;
	NWK_Init(2);
	//clrscr();
	APS_init();
	APS_RegisterSapHandlers(&APME_ZDO_Saphandler, &MSGfromAPStoAPP);
	ZDO_Start(2);

	printf("Bulb is : %s\n", status==1 ? "On" : "Off" );

	while(1);
	return 0;
}


void MSGfromAPStoAPP( DataIndication_t  Msg )
{
	void *p;
	struct command response;
	response.clusterId = ZCL_CLUSTERID_ONOFF;
	p = (void *)Msg.ADSU;

	zclDecoder( &response ,&p,(int *) &Msg.ADSULength );

	act(&response);
}

void act(struct command *cmd)
{
	if( cmd->clusterSpecific == 1 )
	{
		switch (cmd->commandId){
			case ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_OFF: 
				printf("A command to turn off the bulb recieved\n");
				if(status)
				{ 
					printf("bulb turned off\n");  
					status= 0; 
				}
				else 
				{
					printf("bulb is already turned off\n");
				}
				break;
			case ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_ON:
				printf("A command to turn on the bulb recieved\n");
				if(!status)
				{ 
					printf("bulb turned on\n"); 
					status = 1; 
				}
				else 
				{
					printf("bulb is already turned on\n");
				}
				break;
			case ZCL_ATTRIBUTE_CLUSTER_SPECIFIC_ONOFF_TOGGLE:
				printf("A command to toggle the bulb recieved\n");
				if(status)
				{ 
					printf("bulb turned off\n"); 
					status = 0 ; 
				}
				else 
				{ 
					printf("bulb turned on\n"); 
					status = 1 ; 
				}
				break;
			default : printf("no match found\n");
		}
	}
	else
	{
		if(cmd->commandId==0)
		{
			PRINTD("read status request came...current status : %s\n",status?"On":"Off");
			cmd->commandId=READ_ATTRIBUTE_RESPONSE_ID;
			cmd->data=status;
			void *p;
			int len;
			zclEncoder(cmd,&p,&len);
			struct DATA_Request readResponse;
			readResponse.DstAddrMode = 0x02;
			readResponse.DstAddress =0x00;
			readResponse.DstEndpoint = 0x01;
			readResponse.ProfileId = 0x0001;
			readResponse.SrcEndpoint = 0x01;
			readResponse.TxOptions = 0x04;
			readResponse.RadiusCounter = 0xff;
			readResponse.ClusterId = ZCL_CLUSTERID_ONOFF;
			memcpy(readResponse.ADSU,(void *)p,len);
			readResponse.ADSULength = len;
			APSDE_DATA_Request(&readResponse);



		}
	}
}
