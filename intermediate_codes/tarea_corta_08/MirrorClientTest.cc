/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example
 *
 **/

#include <stdio.h>
#include "Socket.h"

#define PORT 5678
#define BUFSIZE 512

int main( int argc, char ** argv ) {
   Socket s('s', true );     // Crea un socket de IPv4, tipo "stream"
   char buffer[ BUFSIZE ];

   s.Connect( "ip address in dot decimal format", PORT ); // Same port as server
   if ( argc > 1 ) {
      s.Write( argv[1] );		// Send first program argument to server
   } else {
      s.Write( "Hello world 2023 ..." );
   }
   s.Read( buffer, BUFSIZE );	// Read answer sent back from server
   printf( "%s", buffer );	// Print received string

}

