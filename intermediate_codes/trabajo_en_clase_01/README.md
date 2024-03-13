[Trabajo en clase: 2023/Ago/16; entrega 2023/Ago/23]

Objetivo
   Construir una jerarquía de clases "VSocket" y "Socket" en C++ para lograr el intercambio de mensajes entre
 computadoras que no comparten memoria 

Funcionamiento

   Vamos a trabajar con la comunicación entre varias máquinas, que no comparten memoria y que podemos
   denominar "Memoria Distribuida".  Siempre vamos a tener dos (o más) procesos que se comunican entre sí, pero
   ahora residen en máquinas distintas con arquitecturas que pueden diferir, que inclusive pueden tener sistemas
   operativos diferentes, pero que deben compartir un mismo protocolo de comunicación para que puedan entenderse.

   Uno de los esquemas para lograr esta comunicación es "Cliente-Servidor", en el que uno de los procesos en una 
   computadora lo denominaremos "cliente" y al otro, en otra computadora, "servidor".  Requerimos que ambas
   computadoras estén enlazadas por un canal de comunicación (red local).  Como indicamos antes, no es necesario
   que ambos programas corran bajo el mismo sistema operativo.

   En esta primera entrega, vamos a crear los métodos de una clase "Socket" en C++ para lograr que un proceso cliente
   se pueda conectar a un servidor.  En una segunda entrega, vamos a completar esta clase "Socket" con otros
   métodos para que tenga la funcionalidad requerida por un servidor.

   Paso de mensajes por medio de sistemas Linux, descripción lógica:

      ________________                                ________________
     |                |                              |                |
     |   -----------  |                              |   -----------  |
     |  |           | |                              |  |           | |
     |  | Proceso C | |                              |  | Proceso S | |
     |  |  Cliente  | |                              |  | Servidor  | |
     |   -----------  |                              |   -----------  |
     |       |        |                              |       |        |
     |  ============  |                              |  ============  |
     |  +          +  |         /~~~~~~~~~~\         |  +          +  |
     |  + Sist. Op.+  |         |  Red de  |         |  + Sist. Op.+  |
     |  +  (Linux) +  |&lt;-------&gt;|  comunic.|&lt;-------&gt;|  + (Linux)  +  |
     |  ============  |         \~~~~~~~~~~/         |  ============  |
     |   Máquina A    |                              |   Máquina B    |
     |                |                              |                |
     |________________|                              |________________|


   Para lograr la comunicación del proceso C con el proceso S, es necesario contar con la dirección del equipo
   donde reside el proceso S, o sea B, además requerimos la ubicación del proceso S.  En la jerga de redes de 
   comunicación, la ubicación del proceso S es denominado puerto.  Por lo tanto, para establecer la comunicación
   entre estos dos procesos, vamos a requerir ambos componentes: dirección de la máquina y dirección del proceso.
   Deben estudiar los llamados al sistema indicados para entender como desarrollar esta clase.

Tareas

   - Completar la clase C++ "VSocket" para poder intercambiar mensajes entre procesos que no comparten memoria.
     Se facilita la interfaz "VSocket.h".  Para la primera etapa deben completar:

        VSocket, el constructor (socket)
        Close, para destruir el Socket (close)
        virtual Connect, para conectarse (connect)
        virtual Read, para leer información por el Socket (read)
        virtual Write, para escribir por el Socket (write)

   - Instancias de la clase:
     protected:
        int idSocket;	// Socket "file" descriptor
        bool ipv6;	// If working with IPv6 socket
        int port;	// Associated port

   - Los ejemplos "ipv4-test.cc" y "ipv6-test.cc" deben funcionar correctamente

   - Una vez construido el socket (socket), Unix lo interpreta como un archivo abierto por lo que es posible realizar
     lecturas (read) y escrituras (write) a ese "archivo"


Descripción detallada
   - Clase VSocket

      InitVSocket( char, bool = false );	// char = 's' for stream, 'd' for datagram, IPv6 = true if required

      ~VSocket();			// call Close class method

      Close();				// close socket

      DoConnect( char * hostip, int port );		// connect to server (char *), and port (int)

         // Como utilizar la estructura de conexión, consultar el manual
         // Para IPv4
            int st;
            struct sockaddr_in  host4;
            memset( (char *) &amp;host4, 0, sizeof( host4 ) );
            host4.sin_family = AF_INET;
            st = inet_pton( AF_INET, hostip, &amp;host4.sin_addr );
            if ( -1 == st ) {
               throw( std::runtime_error( "VSocket::DoConnect, inet_pton" ) );
            }
            host4.sin_port = htons( port );
            st = connect( idSocket, (sockaddr *) &amp;host4, sizeof( host4 ) );
            if ( -1 == st ) {
               throw( std::runtime_error( "VSocket::DoConnect, connect" ) );
            }

      virtual int Connect( const char *, int ) = 0;

      virtual size_t Read( void *, size_t ) = 0;

      virtual size_t Write( const void *, size_t ) = 0;


   - Clase Socket

      Socket( char, bool = false );

      int Connect( const char *, int );

      size_t Read( void *, size_t );

      size_t Write( const void *, size_t );



Referencias
   Recuerde que puede utilizar el manual (man) de los Unix para consultar los llamados al sistema indicados:
      socket
      close
      connect	(man connect)
      read	(man 3 read)
      write	(man 3 write)
      inet_pton
      htons

   https://os.ecci.ucr.ac.cr/ci0123/Asignaciones/SocketsCourse.ppt