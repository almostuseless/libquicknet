#ifndef NET_H_GUARD
	#define NET_H_GUARD 0

	/* Prototypes */

	void destroySocket( unsigned short sockfd );
	short createTcpSocket( const char *host, const char *port, int recv_timeout );
	short sendSocket( unsigned short sockfd, const char *data );
	int socksInit(const char *host, const char *port, const char *phost, const char *pport );

	/* End prototypes */

	typedef struct _SOCKS_INFO
	{
		char 	vn;		// SOCKS Version
		char 	cd;		// Command code
		char 	rsv;		// Reserved, must be 0x00
		char 	atype;		// Address type ( 0x01: IPv4, 0x03: Hostname, 0x04: IPv6 )
		int 	ipv4addr;
		char 	*hostAddr;	// 1st byte: length, .. hostname
		short	byteorder;
	
	} _SOCKS_INFO; //  _attribute( ( __packed__ ) );


#endif

