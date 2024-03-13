/**
 *
 *   UCR-ECCI
 *
 *   SSL Socket class interface
 *
 **/

#ifndef SSLSocket_h
#define SSLSocket_h

#include "VSocket.h"


class SSLSocket : public VSocket {

   public:
      SSLSocket( bool IPv6 = false );				// Not possible to create with UDP, client constructor
      SSLSocket( char *, char *, bool = false );		// For server connections
      SSLSocket( int );
      ~SSLSocket();
      int Connect( const char *, int );
      int Connect( const char *, const char * );
      size_t Write( const char * );
      size_t Write( const void *, size_t );
      size_t Read( void *, size_t );
      SSLSocket * Accept();
      void ShowCerts();
      const char * GetCipher();

   private:
      void Init( bool = false );			// Defaults to create a client context, true if server context needed
      void InitContext( bool = false);
      void LoadCertificates( const char *, const char * );

// Instance variables      
      void * SSLContext;					// SSL context
      void * SSLStruct;					// SSL BIO (Basic Input/Output)

};

#endif

