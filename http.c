#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#ifdef __WIN32
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
#include "global.h"
#include "http.h"


HTTP_REQUEST *httpRequest( const char *host, const char *port, const char *method, const char *request, double max_len )
{
	int 		sockfd;
	int 		recvSize;
	int 		recvTotal 	= 0;
	int		reqLen		= ( strlen( host ) + strlen( method ) + strlen( request ) ) + 25;
	int		x;
       	char 		tBuf[ 512 ] 	= { 0 };
	char    	*recvBuf 	= calloc( max_len * 1024, sizeof( char ) );
	char 		*reqBuf 	= calloc( reqLen, sizeof( char ) );
	static 		HTTP_REQUEST	httpRet;	

	if( ( sockfd = createTcpSocket( host, port, 2 ) ) < 0 )
		return( &httpRet );

	sprintf( reqBuf, "%s %s HTTP/1.1\r\nHost: %s\r\n\r\n", method, request, host );

	sendSocket( sockfd, reqBuf );

	free( reqBuf );

	max_len *= 1024;



	while( ( recvSize = recv( sockfd, tBuf, sizeof( tBuf ) - 1, 0 ) ) >= 1 )
	{

		if( ( recvTotal += recvSize ) >= max_len )
			break;

		strncat( recvBuf, tBuf, max_len - strlen( recvBuf ) );
		
	}

	char *body = strstr( recvBuf, "\r\n\r\n" ) + 4;


	httpRet.blength = strlen( body ) + 1;

	httpRet.body = calloc( httpRet.blength,  sizeof( char ) );

	strcpy( httpRet.body, body );
	httpRet.hlength = ( strlen( recvBuf ) - strlen( httpRet.body ) ) - 4;

	httpRet.headers = calloc( httpRet.hlength, sizeof( char ) );

	sscanf( httpRet.headers, "HTTP %u", &httpRet.status_code );

	for( x = 0; x < httpRet.hlength; x++ )
		httpRet.headers[ x ] = recvBuf[ x ];

	free( recvBuf );

	return( &httpRet );
}


char *httpGetHeader( HTTP_REQUEST *req, const char *header )
{
	char 	*search 	= strstr( req->headers, header ) + strlen( header ) + 2;
	char	*retBuf 	= calloc( 1024, sizeof( char ) );	// shit out of luck if longer

	int x;

	for( x = 0; x < 1024; x++ )
	{
		if( ( search[x] == '\r' ) && ( search[x+1] == '\n' ) )
		{
			break;
		}
		retBuf[x] = search[x];
	}


	return( retBuf );
}


















