/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/
 
#include <iostream>
#include <thread>

#include "VSocket.h"
#include "Socket.h"

#define PORT 5678
#define BUFSIZE 512


/**
 *   Task each new thread will run
 *      Read string from socket
 *      Write it back to client
 *
 **/
void task( Socket * client ) {
   char a[ BUFSIZE ];

   client->Read( a, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
   std::cout << "Server received: " << a << std::endl;
   client->Write( a );		// Write it back to client, this is the mirror function
   client->Close();		// Close socket in parent

}

/**
 *   Create server code
 *      Infinite for
 *         Wait for client conection
 *         Spawn a new thread to handle client request
 *
 **/
int main( int argc, char ** argv ) {
   std::thread * worker;
   Socket * s1;
   Socket * client;

   s1 = new Socket( 's', true );

   s1->Bind( PORT );		// Port to access this mirror server
   s1->Listen( 5 );		// Set backlog queue to 5 conections

   for( ; ; ) {
      client = s1->Accept();	 	// Wait for a client conection
      worker = new std::thread( task, client );
   }

}

