#include <ctype.h>
#include "myutility.h"
#include "light.h"
#include<inttypes.h>
#include "NWK.h"
#include "APS.h"
#include "zdo.h"
#include "zcl.h"

void MSGfromAPStoAPP( DataIndication_t Msg )
{
	void *p;
    struct command response;
    response.clusterId = ZCL_CLUSTERID_ONOFF;
	p = (void *)Msg.ADSU;
	zclDecoder( &response ,&p,(int *) &Msg.ADSULength );
	printf("current status of Bulb %s\n",response.data ? "On" : "Off");
}

int main()
{
   // clrscr();
    NWK_Init(1);
    APS_init();
	APS_RegisterSapHandlers(&APME_ZDO_Saphandler, &MSGfromAPStoAPP, &APME_Deviceremove_Saphandler);
	ZDO_Start(1);
    int i,x=1,index,type;
    char c;
    deviceList *list;
    
   /* device_data[0]=malloc(sizeof(BulbData));   
    device_data[1]=malloc(sizeof(BulbData));   
    device_data[2]=malloc(sizeof(BulbData)); */ 
    flush(); 
    while(x)
    {
        list = getDevice();
      //  clrscr();
        printf("Press D to get avalible device list\n");
        if(list == NULL)
        {
            printf("No device bound till now\n");
        }
        else
        {
            printf("Press C to controll devices\n");
        }
        printf("Press E to exit\n");
        printf("Please Enter your choice ");
        scanf(" %c",&c);
		c=toupper(c);
        flush();
    
        switch(c)
        {
            case 'D':
				display(list);
                break;
            case 'C':
              //  clrscr();
                if(list == NULL)
                {
                    printf("No data\n");
                    //end();
                    break;
                }
                i=0;
                while(list != NULL)
                {
                    printf("Press %d to access Device: %s (with device_id %d)\n",i,list->deviceType,list->deviceId);
                    list = list->next;
                    i++;
                }
				do{
					scanf("%d",&index);
					if( index >= i )
						printf("Invalid choice\n");
                	flush();					
					}while( index >= i );
			
                i=0;
                
                list = getDevice();
				//display(list);
                while(list!=NULL)
                {
                    if(i==index)
                    {
                        type = list->clusterId;
                        PRINTD("Type matched\n");
                        break;
                    }
                    list = list->next;
                    i++;
                }
                PRINTD("Type : %d \n",type);
                switch(type)
                {
                    case 6:
                        PRINTD("in 1\n");
                        if(list==NULL)
                        {
                            printf("Error is null\n");
                        }
                        PRINTD("In switch 1 id %d\n",list->deviceId);
                        smart_bulb(list->deviceId);
                        break;
                    default:
                        printf("Not found\n");
                        break;
                }
                break;
            case 'E':
                x=0;
                break;
            default:
                printf("Wrong Input\n");
                //end();
                break;
        }
    }
    return 0;
}
