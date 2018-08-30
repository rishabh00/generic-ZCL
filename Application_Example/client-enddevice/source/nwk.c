/*****************************************************************************
 * Description:
 * @file nwk.h 
 * @brief: Defination & decleration for network management 
 *    
 * Author       - Rishabh S Sheth
 *
 ***********************************************************************************
 *
 * History
 *                      
 * March/16/2017, Rishabh S, Created 
 *
 ***********************************************************************************/

/***********************************************************************************
 * Include
 ************************************************************************************/
#include "NWK.h"

/************************************************************************************
 * Define,structure
 ************************************************************************************/
#define MAXSIZE 30
#define MAXDEVICE 5

/*  structure   */
typedef struct list
{
	int client_sid;
	uint16_t  device_id;
	char client_ip[ MAXSIZE ]; 
	struct list *next;
} DeviceList;

typedef enum {
	NWK_SUCCESS,
	NWK_FAILED,
}NWK_Respnose_t;

/*********************************************************************************************
 * Global variables
 *********************************************************************************************/
sem_t mutex;
int client_socket;
int id;
struct list *g_root; 
int socket_desc;
int g_device_list[MAXDEVICE]; 


void (*MCPS_ZCL_Handler)( NwkMsg_t );
/************************************************************************************************
 * Function prototype
 ************************************************************************************************/
void* ServerInit( void *msg );         /* Intialise the server network */
int ClientInit(void);        /* Intialise the Client network */
void *RecvFromClient(void *data);      /* Thread for Receive message from client */
void *RecvFromServer( void *data );    /* Thread for Receive message from server */
void RemoveDeviceFromList( int id );   /* Remove device from device table */
void AddDeviceInList( int id, char ip[] );  /* Add device in device Table */
int FindId();                            /* Find the unique device id for connected device */
uint16_t GiveId(int client_socket); /*It will give device Id for specific socket number*/ 



/****************************************************************************************************
 * Function definition
 *****************************************************************************************************/
void NW_RegisterSapHandlers(void (*MCPS_ZCL_SapHandler)( NwkMsg_t msg ))
{
	/* MCPS_ZCL_SapHandler(); */
	MCPS_ZCL_Handler = MCPS_ZCL_SapHandler;
} 

uint16_t GiveId(int client_socket)
{
	DeviceList *tmp = g_root;
	while(tmp!=NULL)
	{
		if( tmp->client_sid == client_socket )
		{
			return tmp->device_id;
		}
		tmp = tmp->next;
	}
	return 0;
}

void *RecvFromClient( void *data )
{	
	int len;
	uint16_t device_ID;
	int client_sock = *((int*)data);
	NwkMsg_t RecvMessage; 

	if((device_ID = GiveId( client_sock )) == 0)
	{
		printf("No Entry found\n");
		exit(0);
	}

	while ( 1 )
	{
		PRINTD("waiting for client message\n");
		if( ( len = recv( client_sock, &RecvMessage, sizeof( NwkMsg_t ), 0 ) ) < 0 )
		{
			printf("recv failed\n");
			break;
		}
		else if( len == 0 )
		{
			printf("Client disconnected\n");
			RemoveDeviceFromList( client_sock );   /* remove entry of client from device table */
			break;   
		}

		RecvMessage.device_id = device_ID ;

		MCPS_ZCL_Handler( RecvMessage );    /* callback function */
	}
	close( client_sock );
	pthread_exit( NULL );
}

void *RecvFromServer( void *data )
{    
	int sock = client_socket;
	int len;
	uint16_t IdDevice;
	NwkMsg_t RecvMessage;
	/* Receive a reply from the server */
	while( 1 )
	{
		if( ( len = recv( sock, &RecvMessage, sizeof(NwkMsg_t), 0 ) ) < 0 )
		{
			printf("recv failed\n");
			break;
		}
		else if( len == 0 )
		{
			printf("Server disconnected\n");
			close( sock );
			exit( EXIT_FAILURE );
		} 
		MCPS_ZCL_Handler( RecvMessage );
	}

	close( sock );
	pthread_exit( 0 );
}

void AddDeviceInList( int id, char ip[] )
{
	struct list *tmp = g_root;
	int number;

	number = FindId();          /* call function to find unique device id */
	if( number == 0 )
	{
		printf("Device can not connect\n");
		return;
	}

	if( g_root == NULL )
	{
		g_root = malloc( sizeof( struct list ) );
		g_root->client_sid = id;
		g_root->device_id = number;
		strcpy(g_root->client_ip , ip);
		g_root->next = NULL;
		return ;
	}    

	while( tmp->next != NULL )   /* traverse the list to find end node */
	{
		tmp = tmp->next;
	}

	tmp->next = malloc( sizeof( struct list ) );
	tmp->next->client_sid = id;
	tmp->next->device_id = number;
	strcpy( tmp->next->client_ip, ip );
	tmp->next->next = NULL;
	return;
}

void RemoveDeviceFromList( int client_sock )
{
	int index = 0;
	struct list *ptr = g_root, *tmp = NULL ;

	while (  ptr != NULL )
	{
		if( ptr->client_sid  == client_sock )
		{
			if( tmp == NULL)
			{
				g_root = g_root->next;
			}
			else
			{
				tmp->next  = ptr->next;
			}

			index = ptr->device_id;
			g_device_list[ --index ] = 0;
			free( ptr );
			printf("\nsocket with socketid %d deleted.\n", client_sock );
		}
		tmp = ptr;
		ptr = ptr->next;
	}       
}

int FindId()
{
	int index;
	for ( index = 0; index < 5; index++ )
	{
		if ( g_device_list[index] == 0 )
		{
			g_device_list[index] = 1;
			return ( index + 1 );
		}
	}
	return 0;
} 

int APDE_NWK( NwkMsg_t *msg )
{
	if( id == 1 )
	{
		int i = 0;
		struct list *tmp;
		tmp = g_root;

		while( tmp != NULL )
		{
			if( msg->device_id == 0 )
			{
				send( tmp->client_sid, msg, sizeof(NwkMsg_t), 0 );
				tmp = tmp->next;
				i = 1;
			}
			else if( msg->device_id == tmp->device_id )
			{
				send( tmp->client_sid, msg, sizeof(NwkMsg_t), 0 );
				i = 1;
				break;
			}
			else
			{
				tmp = tmp->next; 
			}
		}
		PRINTD("at end of send\n");
		return i;
	}
	else if( id == 2 )
	{
		send( client_socket, msg, sizeof(NwkMsg_t), 0);
		return 1;
	}
}

void *ServerInit( void *msg )
{
	int port_add;
	struct sockaddr_in server, client;
	struct list *g_root = NULL;
	int client_sock, ret1, ret2, flag = 0;
	pthread_t send_thread, recv_thread; 
	socket_desc = socket( AF_INET , SOCK_STREAM , 0);
	if ( socket_desc == -1 )
	{
		printf("Could not create socket\n");
		sem_post( &mutex );
		exit( 0 );
	}
	PRINTD("Socket created\n");
	printf("Enter port number : ");
	scanf("%d",&port_add);
	/* Prepare the sockaddr_in structure */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port_add );

	/* Bind  */
	if( bind( socket_desc,( struct sockaddr * )&server , sizeof( server ) ) < 0 )
	{
		/* print the error message */
		perror("bind failed. Error");
		close( socket_desc );
		sem_post( &mutex );
		exit( EXIT_FAILURE );
		pthread_exit(0);
	}

	printf("bind done\n");
	unsigned int c = sizeof( struct sockaddr_in );   

	/* Listen */
	listen( socket_desc , 3 );

	while ( 1 )
	{
		/* Accept and incoming connection */
		printf("Waiting for incoming connections...\n");
		sem_post( &mutex );
		/* accept connection from an incoming client */
		client_sock = accept( socket_desc, ( struct sockaddr * )&client, (socklen_t*)&c);
		if ( client_sock < 0 )
		{
			perror("accept failed\n");
			break;
		}

		AddDeviceInList( client_sock,inet_ntoa( client.sin_addr ));   /* add the entry of incoming device in device table */
		printf("Connection accepted\n");

		ret2 = pthread_create( &recv_thread, NULL, RecvFromClient, &client_sock );
		if( ret2 != 0 )
		{
			printf("Error in creating thread\n");
			exit( 1 );
		}	    
	}
	pthread_exit(0);
}

int ClientInit( void )
{
	char ip[20];
	int sno;
	pthread_t client_recive;
	struct sockaddr_in client;
	printf("Enter ip address : ");
	scanf("%s",ip);
	printf("Enter port number : ");
	scanf("%d",&sno);

	/*  Create socket  */
	client_socket = socket( AF_INET , SOCK_STREAM , 0 );
	if ( client_socket == -1 )
	{
		printf("Could not create socket\n");
		exit( EXIT_FAILURE );
	}
	printf("Socket created\n");    

	client.sin_addr.s_addr = inet_addr( ip );
	client.sin_family = AF_INET;
	client.sin_port = htons( sno );

	/*  Connect to remote server  */
	if ( connect( client_socket ,( struct sockaddr * ) &client , sizeof( client ) ) < 0 )
	{
		perror("connect failed. Error ");
		exit( EXIT_FAILURE );
	}
	pthread_create(&client_recive, NULL, &RecvFromServer, NULL);
	printf("Connected\n");
	return 1;
}

int NWK_Init( int type )
{
	sem_init(&mutex,0,1);
	switch( type )
	{
		case 1:
			id = type;
			sem_wait(&mutex);
			pthread_t server;  /*Thread ID for creating server. */
			pthread_create( &server, NULL, &ServerInit, NULL ); /* create thread for network layer */ 
			break;
		case 2:
			id = type;
			ClientInit();
			break;
		default :
			id  = 0;
			printf("Wrong Input\n");
			break;
	}
	sem_wait(&mutex);
	sem_destroy(&mutex);
	return 0;
}
