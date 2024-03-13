/**
  *
  *   Client side implementation of UDP client-server model 
  *
  *   CI-0123 Proyecto integrador de redes y sistemas operativos
  *
 **/

#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "VSocket.h"
#include "Socket.h"

#define PORT    6789 
#define MAXLINE 1024 

int main() {
   VSocket * client;
   int sockfd; 
   int n, len; 
   char buffer[MAXLINE]; 
   char *hello = (char *) "Hello from CI0123 client"; 
   struct sockaddr_in other;

   client = new Socket( 'd' );	// Creates an UDP socket: datagram

   memset( &other, 0, sizeof( other ) ); 
   
   other.sin_family = AF_INET; 
   other.sin_port = htons(PORT); 
   other.sin_addr.s_addr = INADDR_ANY; 
   
   n = client->sendTo( (void *) hello, strlen( hello ), (void *) & other ); 
   printf("Client: Hello message sent.\n"); 
   
   n = client->recvFrom( (void *) buffer, MAXLINE, (void *) & other );
   buffer[n] = '\0'; 
   printf("Client message received: %s\n", buffer); 

   client->Close(); 

   return 0;
 
} 

