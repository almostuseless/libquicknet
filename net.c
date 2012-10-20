#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifdef _WIN32
	#include <windows.h>
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/wait.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/wait.h>
	#include <netdb.h>
#endif


#include "error.h"
#include "net.h"


short createTcpSocket( const char *host, const char *port, int recv_timeout )
{

	int 			sockfd; 
	struct addrinfo 	*res, *res0, hints;

	#ifdef __WIN32
		WSADATA wsaData;
		LPHOSTENT hostEntry;
	if ( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) != 0 ) 
		return( NET_WINSOCK_ERROR );
	#endif

	memset( &hints, 0, sizeof( hints ) );

	hints.ai_flags		= AI_ALL|AI_ADDRCONFIG;
	hints.ai_socktype	= SOCK_STREAM;
	hints.ai_family		= AF_INET;

	if ( getaddrinfo( host, port, &hints, &res0  ) != 0 )
		return( -1 );

	for( res = res0; res != NULL; res = res->ai_next )
	{

		if ( ( sockfd = socket( res->ai_family, res->ai_socktype, res->ai_protocol ) ) < 0 )
			continue;

		if( recv_timeout > 0 )
		{
//			fcntl( sockfd, F_SETFL, O_NONBLOCK );
			struct timeval 		tv;

			tv.tv_sec 	= recv_timeout;
			tv.tv_usec	= recv_timeout * 100000;
			

			setsockopt( sockfd, SOL_SOCKET, SO_RCVTIMEO, ( char *) &tv, sizeof( tv ) );
		}

		if ( connect( sockfd, res->ai_addr, res->ai_addrlen ) != 0 )
		{
			close( sockfd );
			continue;
		}	
		
		break;
	}

	if( res == NULL )
		return( NET_CONNECT_ERROR );

	freeaddrinfo( res0 );

	return( sockfd );
}
			
short sendSocket( unsigned short sockfd, const char *data )
{
	

	if ( send( sockfd, data, strlen( data ), 0 ) != strlen( data ) )
		return( NET_SEND_ERROR );

	return( 0 );
}

void destroySocket( unsigned short sockfd )
{
	#ifdef __WIN32
		WSACleanup();
		closesocket( sockfd );
	#else 
		close( sockfd );
	#endif
}


/* 
 * Todo: make this work..
 *
*/

int socksInit( const char *host, const char *port, const char *phost, const char *pport )
{
	int sockfd = createTcpSocket( phost, pport, 0 );

	char ssend[20] = { 0 };
	char srecv[20] = { 0 };

	ssend[0]	= 0x05;	// SOCKS Version
	ssend[1]	= 0x01;	// Command (new stream)
	ssend[2]	= 0x00;	// Reserved
	ssend[3]	= 0x01;	// Address type

	// Set port number
	short tport = atoi( pport );
	int thost = atoi( phost );

	ssend[4]	= ( tport >> 8 ) & 0xFF;
	ssend[5]	= tport & 0xFF;

	// Set IP
	ssend[6]	= ( thost >> 24 ) & 0xFF;
	ssend[7]	= ( thost >> 16 ) & 0xFF;
	ssend[8]	= ( thost >> 8 ) & 0xFF;
	ssend[9]	= thost & 0xFF;


	send( sockfd, ssend, 10,  0 );

	printf("Got %d bytes\n", (unsigned int)recv( sockfd, srecv, sizeof( srecv ), 0 ) );

		
	return(0);

}


