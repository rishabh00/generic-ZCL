#include <ctype.h>
#include "light.h"
#include "myutility.h"
#include "APS.h"
#include "zcl.h"
#include "global.h"

void smart_bulb(int id)
{
    char choice;
    int i=1;
    void *p;
    device_data[id -1] = malloc(sizeof(BulbData));
    BulbData *l_data;
	
    l_data = (BulbData*)device_data[id-1];
    struct DATA_Request *req = malloc(sizeof(struct DATA_Request));
    struct command cmd;
    void *packet;
    int len;

    req->DstAddrMode = 0x02;
    req->DstAddress =(uint16_t) id;
    printf("Device Address %d\n",req->DstAddress);
    req->DstEndpoint = 0x01;
    req->ProfileId = 0x0001;
    req->SrcEndpoint = 0x01;
    req->TxOptions = 0x04;
    req->RadiusCounter = 0xff;
    req->ClusterId = cmd.clusterId = ZCL_CLUSTERID_ONOFF;
	memset(l_data->name,'\0',sizeof(l_data->name));
    if(l_data == NULL)
    {
        return;
    }

    while(i)
    {
        clrscr();
        if(l_data->name == NULL)
        {
            printf("Unnamed device\n");
        }
        else
        {
            printf("Bulb : %s\n",l_data->name);
        }
        printf("Press N to change Bulb name\n");
        printf("Press S to getstatus of bulb\n");
        printf("Press O to on bulb\n");
        printf("Press F to off bulb\n");
        printf("Press T to toggle bulb\n");
        printf("Press E to exit from this menu\n");
        scanf(" %c",&choice);
		choice=toupper(choice);

       // flush(); 
        switch(choice)
        {
            case 'N':
                printf("Enter Name of Device :\n");
                scanf("%s",l_data->name);
                break;
            case 'S':
                cmd.clusterSpecific=0;
                cmd.commandId = 0;
                cmd.attributeId=0;
                zclEncoder(&cmd,&p,(int*)&req->ADSULength);
                memcpy(req->ADSU,p,req->ADSULength);
                APSDE_DATA_Request(req);
                end();
                break;
            case 'O':
                cmd.clusterSpecific=1;
                cmd.commandId = 1;
                zclEncoder(&cmd,&p,(int*)&req->ADSULength);
                memcpy(req->ADSU,p,req->ADSULength);
                APSDE_DATA_Request(req);
                end();
                break;
            case 'F':
                cmd.clusterSpecific=1;
                cmd.commandId = 0;
                zclEncoder(&cmd,&p,(int*)&req->ADSULength);
                memcpy(req->ADSU,p,req->ADSULength);
                APSDE_DATA_Request(req);
                end();
                break;
            case 'T':
                cmd.clusterSpecific=1;
                cmd.commandId = 2;
                zclEncoder(&cmd,&p,(int*)&req->ADSULength);
                memcpy(req->ADSU,p,req->ADSULength);
                APSDE_DATA_Request(req);
                end();
                break;
            case 'E':
                i=0;
                return;
                break;
            default :
                printf("Wrong Input,Please try again\n");
                end();
                break;
        }
    }
}



