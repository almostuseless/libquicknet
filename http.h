#ifndef __HTTP_H_GUARDED
	#define __HTTP_H_GUARDED 1


	typedef struct _HTTP_REQUEST 
	{
		char 		*headers;
		char 		*body;
		unsigned short 	hlength;
		unsigned short 	blength;
		unsigned int	status_code;
	
	} HTTP_REQUEST;
	
	HTTP_REQUEST *httpRequest( const char *host, const char *port, const char *method, const char *request, double max_len );
	
	char *httpGetHeader( HTTP_REQUEST *req, const char *header );


#endif

