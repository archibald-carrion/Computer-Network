/**
 *   CI0123 PIRO
 *   Clase para utilizar los "sockets" en Linux
 *
 **/

#include <stdio.h>	// for perror
#include <stdlib.h>	// for exit
#include <string.h>	// for memset
#include <arpa/inet.h>	// for inet_pton
#include <sys/types.h>	// for connect 
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/err.h>

#include <iostream>

#include "Socket.h"

/**
  *  Class constructor
  *     use Unix socket system call
  *
  *  @param	char type: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param	bool ipv6: if we need a IPv6 socket
  *
 **/
Socket::Socket( char type, bool IPv6 ){

   this->InitVSocket( type, IPv6 );

}

/**
  *  Class constructor
  *
  *  @param	int descriptor: socket descriptor for an already opened socket
  *
 **/
Socket::Socket( int descriptor ) {

   this->InitVSocket( descriptor );

}


/**
  * Class destructor
  *
 **/
Socket::~Socket(){

   // SSL destroy
   if ( nullptr != this->SSLContext ) {
      SSL_CTX_free( reinterpret_cast<SSL_CTX *>( this->SSLContext ) );
   }
   if ( nullptr != this->SSLStruct ) {
      SSL_free( reinterpret_cast<SSL *>( this->SSLStruct ) );
   }

   Close();
}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
 **/
void Socket::Close(){

}

/**
  * Connect method
  *   use "connect" Unix system call
  *
  * @param	char * host: host address in dot notation, example "10.1.104.187"
  * @param	int port: process address, example 80
  *
 **/
int Socket::Connect( const char * host, int port ) {

   return this->DoConnect( host, port );

}


int Socket::Connect( const char * host, const char * service ) {

   return this->DoConnect( host, service );

}

/**
  * Read method
  *   use "read" Unix system call (man 3 read)
  *
  * @param	void * text: buffer to store data read from socket
  * @param	int size: buffer capacity, read will stop if buffer is full
  *
 **/
size_t Socket::Read( void * text, size_t size ) {
   int st = -1;

   st = read(idSocket, text, size);

   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Read( const void *, size_t )" );
   }

   return st;


}


/**
  * Write method
  *   use "write" Unix system call (man 3 write)
  *
  * @param	void * buffer: buffer to store data write to socket
  * @param	size_t size: buffer capacity, number of bytes to write
  *
 **/
size_t Socket::Write( const void *text, size_t size ) {
   int st = -1;
   st = write(idSocket, text, size);
   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Write( void *, size_t )" );
   }

   return st;
}


/**
  * Write method
  *
  * @param	char * text: string to store data write to socket
  *
  *  This method write a string to socket, use strlen to determine how many bytes
  *
 **/
size_t Socket::Write( const char *text ) {
   int st = -1;
   size_t size = strlen(text);
   st = write(idSocket, text, size);
   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Write( void *, size_t )" );
   }

   return st;
}

/**
  * Accept method
  *    use base class to accept connections
  *
  *  @returns   a new class instance
  *
  *  Waits for a new connection to service (TCP mode: stream)
  *
 **/
Socket * Socket::Accept(){
   int id;
   Socket * other;

   id = this->DoAccept();

   other = new Socket( id );

   return other;

}


/**
  *  SSLInitServerContext
  *     use SSL_library_init, OpenSSL_add_all_algorithms, SSL_load_error_strings, TLS_server_method, SSL_CTX_new
  *
  *  Creates a new SSL server context to start encrypted comunications, this context is stored in class instance
  *
 **/
void Socket::SSLInitServerContext() {
   // const SSL_METHOD * method;

   // method = TLS_server_method();
   // if ( nullptr == method ) {
   //    throw std::runtime_error( "SSLSocket::InitContext( bool )" );
   // }

   // Call SSL_library_init() to register the available SSL/TLS ciphers and digests
   // Call OpenSSL_add_all_algorithms() to load and register all cryptos, etc.
   // Call SSL_load_error_strings() to load all error messages
   // Call TLS_server_method() to create a method
   // Check for errors
   // Call SSL_CTX_new(), as in client, to create a new context
   // Check for errors

   SSL_library_init();
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings();


   // Crea el metodo que se usara para establecer la conexion segura
   const SSL_METHOD* method = TLS_server_method();
   // Se crea un contexto a partir del metodo
   SSL_CTX* context = SSL_CTX_new(method);

   if (!context) {
      perror("Unable to create SSL context");
   }

   this->SSLContext = (void *) context;

   if ( nullptr == method ) {
      throw std::runtime_error( "SSLSocket::InitContext( bool )" );

   }


   // Assign context variable to an instance variable

}



/**
 *  Load certificates
 *    verify and load certificates
 *
 *  @param	const char * certFileName, file containing certificate
 *  @param	const char * keyFileName, file containing keys
 *
 **/
 void Socket::SSLLoadCertificates( const char * certFileName, const char * keyFileName ) {
   SSL_CTX * context = (SSL_CTX *)this->SSLContext;
   int st;

   if ( SSL_CTX_use_certificate_file( context, certFileName, SSL_FILETYPE_PEM ) <= 0 ) {	 // set the local certificate from CertFile
      st = SSL_get_error( (SSL *) this->SSLStruct, st );
      ERR_print_errors_fp( stderr );
      abort();
   }

   if ( SSL_CTX_use_PrivateKey_file( context, keyFileName, SSL_FILETYPE_PEM ) <= 0 ) {	// set the private key from KeyFile (may be the same as CertFile)
      st = SSL_get_error( (SSL *) this->SSLStruct, st );
      ERR_print_errors_fp( stderr );
      abort();
   }

   if ( ! SSL_CTX_check_private_key( context ) ) {	// verify private key
      st = SSL_get_error( (SSL *) this->SSLStruct, st );
      ERR_print_errors_fp( stderr );
      abort();
   }

}


/**
  *  SSLInit
  *     use SSL_new with a defined context
  *
  *  Create a SSL object
  *
 **/
void Socket::SSLInit() {

   SSL * ssl = nullptr;

   this->SSLInitContext();
   ssl = SSL_new( (SSL_CTX *) this->SSLContext );
   if( ssl == nullptr )
   {
      std::cout << "ssl null pointer: Socket::SSLInit()" << std::endl;
   }
   // Check for errors
   this->SSLStruct = (void *) ssl;

   // this->SSLInitContext( serverContext );

   // Call SSLInitContext method to create a new context
   // Create SSL * variable using SSL_new( ... ) call
   // Check for errors

   // Assign SSL * variable to an instance variable

}


/**
  *  SSLInitContext
  *     use TLS_client_method and SSL_CTX_new
  *
  *  Creates a new SSL context to start encrypted comunications, this context is stored in class instance
  *
 **/
void Socket::SSLInitContext() {
   
   // Create a SSL_METHOD * variable using TLS_client_method() call
   // Check for errors on a similar way as showed:
   int st;

   // if ( nullptr == context ) {
   //    ERR_print_errors_fp( stderr );
   //    abort();
   // }

   // Create a new context SSL_CTX * variable using SSL_CTX_new( ... )
   // Check for errors

   // Assign context to an instance variable



   const SSL_METHOD * method;
   SSL_CTX * context;


   // if ( serverContext ) {


   // } else {
      method = TLS_client_method();

      if ( nullptr == method ) {
         throw std::runtime_error( "SSLSocket::InitContext( bool )" );
      }


      context = SSL_CTX_new( method );
      if ( nullptr == context ) {
         throw std::runtime_error( "SSLSocket::InitContext( bool )" );
      }

      this->SSLContext = (void *) context;
   //}
}

/**
 *  SSLInitServer
 *     use SSL_new with a defined context
 *
 *  Create a SSL object for server conections
 *
 *  @param	const char * certFileName, file containing certificate
 *  @param	const char * keyFileName, file containing keys
 *
 **/
void Socket::SSLInitServer( const char * certFileName, const char * keyFileName) {

   this->SSLInitServerContext();
   SSL * ssl = SSL_new( (SSL_CTX *) this->SSLContext );

   if (ssl == nullptr)
   {
      std::cout << "ssl null pointer: Socket::SSLInitServer( const char * certFileName, const char * keyFileName) " << std::endl;
   }
   this->SSLStruct = (void *) ssl;

   SSLLoadCertificates(certFileName, keyFileName);

   // Create a new context calling SSLInitServerContext
   // Create a SSL * variable using SSL_new()
   // Check for errors
   // Assing SSL * variable to an instance variable
   // Load SSL certificates, using SSLLoadCertificates() method

}



/**
 *   Show SSL certificates
 *
 **/
void Socket::SSLShowCerts() {
   X509 *cert;
   char *line;

   cert = SSL_get_peer_certificate( (SSL *) this->SSLStruct );		 // Get certificates (if available)
   if ( nullptr != cert ) {
      printf("Server certificates:\n");
      line = X509_NAME_oneline( X509_get_subject_name( cert ), 0, 0 );
      printf( "Subject: %s\n", line );
      free( line );
      line = X509_NAME_oneline( X509_get_issuer_name( cert ), 0, 0 );
      printf( "Issuer: %s\n", line );
      free( line );
      X509_free( cert );
   } else {
      printf( "No certificates.\n" );
   }

}



/**
 *   SSLCreate constructs a new SSL * variable from a previous created context
 *
 *  @param	Socket * original socket with a previous created context
 *
 **/
void Socket::SSLCreate( Socket * original ) {
   SSL * ssl;
   int st;
   ssl = SSL_new((SSL_CTX *) original->SSLContext);
   if ( nullptr == ssl ) {
      throw std::runtime_error( "SSLSocket::SSLCreate( Socket * ) SSL_new retuned nullptr " );
   }

   this->SSLStruct = (void *) ssl;

   st = SSL_set_fd((SSL *) this->SSLStruct, this->idSocket);


   // Constructs a new SSL * variable using SSL_new() function
   // Check for errors

   // Assign new variable to instance variable

   // change conection status  to SSL using SSL_set_fd() function
   // Check for errors 

}


/**
 *   SSLAccept
 *
 *  waits for a TLS/SSL client to initiate the TLS/SSL handshake
 *
 **/
void Socket::SSLAccept(){
   int st = -1;
   st = SSL_accept((SSL *)this->SSLStruct);
   if (st < 0)
   {
      throw std::runtime_error( "SSLSocket::SSLAccept( void )" );
      /* code */
   }
   
   // Call SSL_accept() to initiate TLS/SSL handshake
   // Check for errors

}

/**
 *   Get SSL ciphers
 *
 **/
const char * Socket::SSLGetCipher() {
   // Call SSL_get_cipher() and return the name
   return SSL_get_cipher( reinterpret_cast<SSL *>( this->SSLStruct ) );
}

/**
  *  Read
  *     use SSL_read to read data from an encrypted channel
  *
  *  @param	void * buffer to store data read
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity read
  *
  *  Reads data from secure channel
  *
 **/
size_t Socket::SSLRead( void * buffer, size_t size ) {
   int st = -1;

   st = SSL_read((SSL *)SSLStruct, buffer, size);
   //  int SSL_read(SSL *ssl, void *buf, int num);

   if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Read( void *, size_t )" );
   }

   return st;

}


/**
  *  Write
  *     use SSL_write to write data to an encrypted channel
  *
  *  @param	void * buffer to store data read
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity written
  *
  *  Writes data to a secure channel
  *
 **/
size_t Socket::SSLWrite( const char * string ) {
   int st = -1;

   st = SSL_write((SSL *)SSLStruct, string, strlen(string));

   if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Write( const char * )" );
   }

   return st;

}


/**
  *  Write
  *     use SSL_write to write data to an encrypted channel
  *
  *  @param	void * buffer to store data read
  *  @param	size_t size, buffer's capacity
  *
  *  @return	size_t byte quantity written
  *
  *  Reads data from secure channel
  *
 **/
size_t Socket::SSLWrite( const void * buffer, size_t size ) {
   int st = -1;

   st = SSL_write((SSL *)SSLStruct, buffer, size);


   if ( st <= 0 ) {
      perror( "SSLSocket::SSLWrite( const void *, size_t )" );
   }

   return st;

}

int Socket::SSLConnect( const char *hostname, int port) {
   int st;

   st = this->DoConnect( hostname, port );		// Establish a non ssl connection first
   if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Connect( const char * hostname, int port ) failt DoConnect" );
      // perror( "SSLSocket::Connect( const char * hostname, int port )" );
   }
   // Call SSL_set_fd
   // SSL *ssl;
   st = SSL_set_fd((SSL *)SSLStruct, this->idSocket);
   if ( st <= 0 ) {
      throw std::runtime_error( "SSLSocket::Connect( const char * hostname, int port ) failed SSL_set_fd" );
      // perror( "SSLSocket::Connect( const char * hostname, int port )" );
   }
   // Call SSL_connect()
   st = SSL_connect((SSL *)SSLStruct);
   // check for errors

  if ( -1 == st ) {
      throw std::runtime_error( "SSLSocket::Connect( const char * hostname, int port )" );
   }

   return st;

}
