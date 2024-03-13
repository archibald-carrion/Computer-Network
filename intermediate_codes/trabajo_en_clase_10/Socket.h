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
 *  Grupo: 2 
 *
 *
 **/

#ifndef Socket_h
#define Socket_h

#include <openssl/ssl.h>

#include "VSocket.h"

class Socket : public VSocket {

   private:
      void SSLInitContext();
      void SSLLoadCertificates(const char * certFileName,
         const char * keyFileName );
      void SSLInitServerContext();

      void * SSLContext;					// SSL context
      void * SSLStruct;					// SSL BIO (Basic Input/Output)

   public:
      Socket( char, bool = false );
      Socket( int );
      ~Socket();
      void Close();
      void SSLInit();
      void SSLInitServer( const char * certFileName, const char * keyFileName);
      int Connect( const char *, int );
      int Connect( const char *, const char* );
      int SSLConnect( const char *, int );
      size_t Read( void *, size_t );
      size_t Write( const void *, size_t );
      size_t Write( const char * );
      Socket * Accept();
      void SSLCreate( Socket * original );
      void SSLAccept();
      const char * SSLGetCipher();
      void SSLShowCerts();
      size_t SSLWrite( const char * );
      size_t SSLWrite( const void *, size_t );
      size_t SSLRead( void *, size_t );
   protected:

};

#endif

