/**
 *
 *   UCR-ECCI
 *
 *   IPv4 TCP client normal or SSL according to parameters
 *
 **/

#include <stdio.h>
#include <string.h>

#include "VSocket.h"
#include "Socket.h"
#include <iostream>
#include "SSLSocket.h"

#define	MAXBUF	1024

/**
**/
int main( int argc, char * argv[] ) {
   VSocket * client;
   int st, port = 80;
   char a[ MAXBUF ];
   char * os = (char *) "163.178.104.187";
   char * lego = (char *) "GET /lego/ HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
//   char * request = (char *) "GET /ci0123 HTTP/1.1\r\nhost:redes.ecci\r\n\r\n";

   if (argc > 1 ) {
      port = 443;
      client = new SSLSocket();	// Create a new stream socket for IPv4
   } else {
      client = new Socket( 's' );
      port = 80;
   }

   memset( a, 0 , MAXBUF );
   client->Connect( os, port );
   client->Write(  (char * ) lego, strlen( lego ) );
   st = client->Read( a, MAXBUF );
   printf( "Bytes read %d\n%s\n", st, a);

}

