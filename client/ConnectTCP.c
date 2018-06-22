/* connectsock.c - connectsock */
#define __USE_BSD 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

/* INADDR_NONE */
typedef unsigned short u_short;
extern int errno;
int errexit(const char *format, ...);
int connectsock(const char *host, const char *service,const char *transport);

/* connectTCP.c - connectTCP */
/*------------------------------------------------------------------------
 * connectTCP - connect to a specified TCP service on a specified host
 *------------------------------------------------------------------------
 */
int connectTCP(const char *host, const char *service )
	/*
	 * Arguments:
	 * host
	 - name of host to which connection is desired
	 * service - service associated with the desired port
	 */
{
	return connectsock( host, service, "tcp");
}


/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP or UDP
 *------------------------------------------------------------------------
 */
int connectsock(const char *host, const char *service, const char *transport )
	/*
	 * Arguments:
	 * host
	 - name of host to which connection is desired
	 * service
	 - service associated with the desired port
	 * transport - name of transport protocol to use ("tcp" or "udp")
	 */
{
	struct hostent *phe; /* pointer to host information entry*/
	struct servent *pse; /* pointer to service information entry */
	struct protoent *ppe; /* pointer to protocol information entry*/
	/*struct sockaddr_in sin; /* an Internet endpoint address*/
	int s, type;
	/* socket descriptor and socket type
	IF IPV4
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
       
	/* Map service name to port number */

       struct sockaddr_in6 serv_addr;
	if ( pse = getservbyname(service, transport) )
		serv_addr.sin6_port = pse->s_port;
	else if ( (serv_addr.sin6_port = htons((u_short)atoi(service))) == 0 )
		errexit("can't get \"%s\" service entry\n", service);

	/* Map host name to IP address, allowing for dotted decimal */
           serv_addr.sin6_flowinfo= 0;
           serv_addr.sin6_family= AF_INET6;
	if ( phe = gethostbyname2(host,AF_INET6) )
		memcpy(&serv_addr.sin6_addr.s6_addr, phe->h_addr, phe->h_length);
	//else if ( (serv_addr.sin6_addr.s6_addr = inet_addr(host)) == INADDR_NONE )
	//	errexit("can't get \"%s\" host entry\n", host);

	/* Map transport protocol name to protocol number */
	if ( (ppe = getprotobyname(transport)) == 0)
		errexit("can't get \"%s\" protocol entry\n", transport);

	/* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

	/* Allocate a socket */
	s = socket(AF_INET6, type, ppe->p_proto);
	if (s < 0)
		errexit("can't create socket: %s\n", strerror(errno));

	/* Connect the socket */
	if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		errexit("can't connect to %s.%s: %s\n", host, service,
				strerror(errno));
	return s;

}

