
/**
 *
 **/

#include <cstdlib>
#include <cstring>	// strlen
#include <cstdio>

#include "Socket.h"

/**
 *
 **/
int main(int cuantos, char * argumentos[] ) {
   Socket * client;
   char userName[16] = { 0 };
   char password[16] = { 0 };
   const char * requestMessage = "\n<Body>\n\
\t<UserName>%s</UserName>\n\
\t<Password>%s</Password>\n\
</Body>\n";

   char buf[1024];
   char clientRequest[ 1024 ] = { 0 };
   int bytes;
   char *hostname, *portnum;
   
   client = new Socket( 's' );
   if ( cuantos != 3 ) {
      printf("usage: %s <hostname> <portnum>\n", argumentos[0] );
      exit(0);
   }
   hostname = argumentos[ 1 ];
   portnum = argumentos[ 2 ];
   client->SSLInit();
   client->SSLConnect( hostname, atoi( portnum ) );
   printf( "Enter the User Name : " );
   scanf( "%s", userName );
   printf( "\nEnter the Password : " );
   scanf( "%s", password );
   sprintf( clientRequest, requestMessage, userName, password );	// construct reply
   printf( "\n\nConnected with %s encryption\n", client->SSLGetCipher() );
   client->SSLShowCerts();		// display any certs
   client->SSLWrite( clientRequest, strlen( clientRequest ) );		// encrypt & send message
   bytes = client->SSLRead( buf, sizeof( buf ) );			// get reply & decrypt
   buf[ bytes ] = 0;
   printf("Received: \"%s\"\n", buf);

   return 0;

}
