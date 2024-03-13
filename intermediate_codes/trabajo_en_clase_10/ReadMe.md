Read.Me Semana 5

[Ejercicio propuesto: 2023/Abr/25; entrega 2023/May/02]
[Modalidad: individual]

Objetivos
   Completar la clase para intercambio de mensajes SSL (TCP IPv4)
   Agregar la funcionalidad del servidor

Introducción
   Vamos a construir un servidor que sea capaz de atender cada solicitud por medio de un recurso separado (hilos o procesos)

Funcionamiento

Tareas

   - Revisar la imagen con el detalle del protocolo SSL

   - Revisar y comprender el ejemplo funcional provisto: SSLClient.c y SSLServer.c
      - Requieren que exista un certificado almacenado en el archivo "ci0123.pem"
      (openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ci0123.pem -out ci0123.pem)

      - Al compilar, deben agregar las bibliotecas para SSL (-lssl -lcrypto)

   - Completar su clase "Socket" para poder intercambiar mensajes por medio de SSL

     Para este trabajo deben completar, algunos de ellos ya pueden estar listos por trabajos anteriores:

     (*** Advertencia: los métodos indicados son sugeridos, pueden realizar su propia implantación ***)

        Socket::SSLInitContext() [privado], inicializa una variable de instancia con el método SSL_CTX_new, la que recibe como
           parámetro un método (SSL_method inicializado con TLS_client_method())

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
...
   int st;

   if ( nullptr == context ) {
      ERR_print_errors_fp( stderr );
      abort();
   }

   // Create a new context SSL_CTX * variable using SSL_CTX_new( ... )
   // Check for errors

   // Assign context to an instance variable


        Socket * Socket::SSLInit(), debe tener un contexto SSL construido, utilizando el método anterior,
           además debe crear una variable SSL * (con SSL_new) que se guarde en la instancia de clase (SSLStruct)

/**
  *  SSLInit
  *     use SSL_new with a defined context
  *
  *  Create a SSL object
  *
 **/
void Socket::SSLInit() {

   // Call SSLInitContext method to create a new context
   // Create SSL * variable using SSL_new( ... ) call
   // Check for errors

   // Assign SSL * variable to an instance variable

}

        Socket::SSLLoadCertificates( const char * certFileName, const char * keyFileName ) [privado], carga los 
           certificados indicados como parámetros

/**
 *  Load certificates
 *    verify and load certificates
 *
 *  @param	const char * certFileName, file containing certificate
 *  @param	const char * keyFileName, file containing keys
 *
 **/
 void Socket::SSLLoadCertificates( const char * certFileName, const char * keyFileName ) {
   // SSL_CTX * context = instance variable
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


        Socket::SSLInitServerContext() [privado], inicializa una variable de instancia con el método SSL_CTX_new, la que recibe como
           parámetro un método (SSL_method inicializado con TLS_client_method())

/**
  *  SSLInitServerContext
  *     use SSL_library_init, OpenSSL_add_all_algorithms, SSL_load_error_strings, TLS_server_method, SSL_CTX_new
  *
  *  Creates a new SSL server context to start encrypted comunications, this context is stored in class instance
  *
 **/
void Socket::SSLInitServerContext() {

   // Call SSL_library_init() to register the available SSL/TLS ciphers and digests
   // Call OpenSSL_add_all_algorithms() to load and register all cryptos, etc.
   // Call SSL_load_error_strings() to load all error messages
   // Call TLS_server_method() to create a method
   // Check for errors
   // Call SSL_CTX_new(), as in client, to create a new context
   // Check for errors

   // Assign context variable to an instance variable

}


        Socket::SSLInitServer(), inicializa una variable de instancia con el método SSL_CTX_new

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
   int st;

   // Create a new context calling SSLInitServerContext
   // Create a SSL * variable using SSL_new()
   // Check for errors
   // Assing SSL * variable to an instance variable
   // Load SSL certificates, using SSLLoadCertificates() method

}


        Socket::SSLShowCertificates() [privado], despliega los certificados identificados en la conexión

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


        Socket::SSLCreate(), construye una nueva variable SSL * a partir de un contexto previamente creado

/**
 *   SSLCreate constructs a new SSL * variable from a previous created context
 *
 *  @param	Socket * original socket with a previous created context
 *
 **/
void Socket::SSLCreate( Socket * original ) {
   SSL * ssl;
   int st;

   // Constructs a new SSL * variable using SSL_new() function
   // Check for errors

   // Assign new variable to instance variable

   // change conection status  to SSL using SSL_set_fd() function
   // Check for errors 

}


        Socket::SSLAccept(), negocia con el cliente la conexión TLS/SSL por medio de un "handshake"

/**
 *   SSLAccept
 *
 *  waits for a TLS/SSL client to initiate the TLS/SSL handshake
 *
 **/
void Socket::SSLAccept(){
   int st = -1;

   // Call SSL_accept() to initiate TLS/SSL handshake
   // Check for errors

}

        Socket::SSLGetCipher(), obtiene el nombre del cifrado que se utiliza en este momento

/**
 *   Get SSL ciphers
 *
 **/
const char * Socket::SSLGetCipher() {

   // Call SSL_get_cipher() and return the name

}



   - Los ejemplos provistos "SSLClient.cc" y "SSLServer.cc" deben funcionar correctamente, requieren que un certificado haya sido creado
     por medio de openssl y almacenado en un archivo "ci0123.pem"

Referencias
   https://os.ecci.ucr.ac.cr/ci0123/material/sockets-course.ppt

