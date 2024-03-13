/**
 *  Establece la definición de la clase Socket para efectuar la comunicación
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de esta clase se hará en varias etapas, primero
 *  los métodos necesarios para los clientes, en la otras etapas los métodos para el servidor,
 *  manejo de IP-v6, conexiones seguras y otros
 *
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2023-ii
 *  Grupos: 2
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			// memset

#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>			// ntohs
#include <unistd.h>			// close
//#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>			// getaddrinfo, freeaddrinfo

#include <iostream>

#include "VSocket.h"


/**
  *  Class initializer
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
void VSocket::InitVSocket( char t, bool IPv6 ) {
   this->IPv6 = IPv6;

   // create socket with IPv6
   if(IPv6) {
      // create socket as stream type
      if(t == 's') {
         this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
      } else {  //  create socket as datagram typé
         this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
      }
   } else {
      // create socket as stream type
      if(t == 's') {
         this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
      } else {  //  create socket as datagram typé
         this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
      }
   }



}


/**
  *  Class initializer
  *
  *  @param     int descriptor: socket descriptor for an already opened socket
  *
 **/
void VSocket::InitVSocket( int descriptor ) {
   this->idSocket = descriptor;
}


/**
  * Class destructor
  *
**/
VSocket::~VSocket() {

   this->Close();

}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
**/
void VSocket::Close(){
   int st;
   st = close(idSocket);
   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Close()" );
   }

}


/**
  * DoConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.1.104.187"
  * @param      int port: process address, example 80
  *
**/
int VSocket::DoConnect( const char * hostip, int port ) {
   int st;
   this->port = port;

   // if ( -1 == st ) {
   //    perror( "VSocket::connect" );
   //    throw std::runtime_error( "VSocket::DoConnect" );
   // }

   if(!IPv6) { // comnnect with IPv++4
      struct sockaddr_in  host4;
      memset( (char *) &host4, 0, sizeof( host4 ) );
      host4.sin_family = AF_INET;
      st = inet_pton( AF_INET, hostip, &host4.sin_addr );
      host4.sin_port = htons( port );
      if ( -1 == st ) {
         throw( std::runtime_error( "VSocket::DoConnect, inet_pton"));
      }
      st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );
     // if ( -1 == st ) {
     //    throw( std::runtime_error( "VSocket::DoConnect, connect" ));
     // }
   } else {  // connect with IPv6
      // similar code but for IPv6
            struct sockaddr_in6  host6;
            struct sockaddr * ha;

            memset( &host6, 0, sizeof( host6 ) );
            host6.sin6_family = AF_INET6;
            st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
            if ( 0 <= st ) {	// 0 means invalid address, -1 means address error
               throw std::runtime_error( "Socket::Connect( const char *, int ) [inet_pton]" );
            }
            host6.sin6_port = htons( port );
            ha = (struct sockaddr *) &host6;
            size_t len = sizeof( host6 );
            st = connect( this->idSocket, ha, len );
            if ( -1 == st ) {
               throw std::runtime_error( "Socket::Connect( const char *, int ) [connect]" );
            }

    }
   
   return st;
}



/** DoConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
  * @param      char * service: process address, example "http"
**/
int VSocket::DoConnect( const char * hostip, const char * service ) {

   std::cout << "start DoConnect" << std::endl;
   struct addrinfo hints, *result, *rp;
	int st;
	
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM; /* Stream socket */
   hints.ai_flags = 0;
   hints.ai_protocol = 0;          /* Any protocol */

   st = getaddrinfo( hostip, service, &hints, &result );

   for ( rp = result; rp; rp = rp->ai_next ) {
	std::cout << "hello" << std::endl;
      st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
      if ( 0 == st) {
         break;
      }
   }
   
   std::cout << "end DoConnect" << std::endl;

   freeaddrinfo( result );
   return st;   
}


/**
  * Listen method
  *
  * @param      int queue: max pending connections to enqueue (server mode)
  *
  *  This method define how many elements can wait in queue
  *
 **/
int VSocket::Listen( int queue ) {
   int st = -1;

   // int listen(int sockfd, int backlog);

   st = listen(this->idSocket, queue);

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::Listen( int )" );
   }

   return st;

}


/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param      int port: bind a unamed socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int VSocket::Bind( int port ) {
   int st = -1;

   if(IPv6) { 
      struct sockaddr_in6 addr;
      memset(&addr, 0, sizeof(addr));
      addr.sin6_family = AF_INET6;
      addr.sin6_addr = in6addr_any;
      addr.sin6_port = htons(port);
      
      bind(idSocket, (struct sockaddr*)&addr, sizeof(addr));
   } else {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = INADDR_ANY;
      
      bind(idSocket, (struct sockaddr*)&addr, sizeof(addr));
   }
   std::cout << "Bind completed" << std::endl;

   return st;
}


/**
  *  sendTo method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to send data
  *
  *  Send data to another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
   int st = -1;

   const struct sockaddr* addrss = (const struct sockaddr *)addr;

   // std::cout << sizeof(addr) << std::endl;
   // std::cout << sizeof(*addrr) << std::endl;

   st = sendto(idSocket, buffer, size, 0,
      addrss, (socklen_t) sizeof(*addrss));

   if ( -1 == st ) {
   //if ( 0 > st ) {
      throw std::runtime_error( "sendto error" );
   }

   return st;
}


/**
  * DoAccept method
  *    use "accept" Unix system call (man 3 accept) (server mode)
  *
  *  @returns   a new class instance
  *
  *  Waits for a new connection to service (TCP mode: stream)
  *
 **/
int VSocket::DoAccept(){
   int st = -1;

   st = accept(this->idSocket, NULL, 0);

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::DoAccept()" );
   }

   return st;
}


/**
  * Shutdown method
  *    use "shutdown" Unix system call (man 3 shutdown)
  *
  *  @param	int mode define how to cease socket operation
  *
  *  Partial close the connection (TCP mode)
  *
 **/
int VSocket::Shutdown( int mode ) {
   int st = -1;

   st = shutdown(this->idSocket, mode);

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::Shutdown( int )" );
   }

   return st;
}


/**
  *  recvFrom method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to receive from data
  *
  *  @return	size_t bytes received
  *
  *  Receive data from another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
   int st = -1;
	
   struct sockaddr * addrss = (struct sockaddr *) addr;
   socklen_t lenAddrss = sizeof(*addrss);


   st = recvfrom( idSocket, buffer, size, 0,
      addrss, &lenAddrss);

   if ( -1 == st ) {
      throw std::runtime_error( "recvfrom error" );
   }
   
   return st;

}

