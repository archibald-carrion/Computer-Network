/*
 *  Esta prueba solo funciona utilizando un equipo de la red interna de la ECCI, por lo que
 *  deberan conectarse por la VPN para realizarla
 *  La direccion IPv6 provista es una direccion privada
 *  Tambien deben prestar atencion al componente que esta luego del "%" en la direccion y que hace
 *  referencia a la interfaz de red utilizada para la conectividad
 *
 *  Pueden probar con telnet para asegurarse de que hay conectividad:
 * 
 *    telnet fe80::194d:4e78:60c:fedc%eno1 80
 *
 */
#include <stdio.h>
#include <string.h>
#include "Socket.h"
#include <iostream>

int main( int argc, char * argv[] ) {
   const char * os = "fe80::194d:4e78:60c:fedc%eno1";
   const char * request = "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', true );
   char a[512];

   memset( a, 0, 512 );
   s.Connect( os, (char *) "http" );
   s.Write(  request );
   s.Read( a, 512 );
   printf( "%s\n", a);
   
   return 0;
}

