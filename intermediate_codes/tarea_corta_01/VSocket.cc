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
 *  Grupos: 2 y 3
 *
 * (versión Fedora)
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			// memset

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
void VSocket::InitVSocket( char t, bool IPv6 ){
   // source: man socket
   //  The socket has the indicated type, which specifies the communication semantics.  Currently defined types are:
   //     SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based byte streams.  An out-of-band data transmission mechanism may be supported.
   //     SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
   //     SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-based data transmission path for datagrams of fixed maximum length; a consumer is required to read an entire packet with each input system call.
   //     SOCK_RAW        Provides raw network protocol access.
   //     SOCK_RDM        Provides a reliable datagram layer that does not guarantee ordering.
   //     SOCK_PACKET     Obsolete and should not be used in new programs; see packet(7).
   //     Some socket types may not be implemented by all protocol families.
   //     Since Linux 2.6.27, the type argument serves a second purpose: in addition to specifying a socket type, it may include the bitwise OR of any of the following values, to modify the behavior of socket():
   //     SOCK_NONBLOCK   Set the O_NONBLOCK file status flag on the open file description (see open(2)) referred to by the new file descriptor.  Using this flag saves extra calls to fcntl(2) to achieve the same result.
   //     SOCK_CLOEXEC    Set the close-on-exec (FD_CLOEXEC) flag on the new file descriptor.  See the description of the O_CLOEXEC flag in open(2) for reasons why this may be useful.
   // int socket(int domain, int type, int protocol);

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
      } else {  //  creadR@j0m727te socket as datagram typé
         this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
      }
   }
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
   int st = 0;

   //std::cout << "closing: " << this->idSocket << std::endl;

   st = close(idSocket);

   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Close()" );
   }

}


/**
  * Connect method
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
      if ( -1 == st ) {
         throw( std::runtime_error( "VSocket::DoConnect, connect" ));
      }
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

int VSocket::DoConnect( const char * hostip, const char * service ) {
   struct addrinfo hints, *result, *rp;
	int st;
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM; /* Stream socket */
   hints.ai_flags = 0;
   hints.ai_protocol = 0;          /* Any protocol */

   st = getaddrinfo( hostip, service, &hints, &result );

   for ( rp = result; rp; rp = rp->ai_next ) {
      st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
      if ( 0 == st) {
         break;
      }
   }

   freeaddrinfo( result );
   return st;   
}

