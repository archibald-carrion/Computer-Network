# Modus operandi para crear certificado autofirmado Apache
## Comandos pre-requisitos
### Instalar Apache
Para instalar Apache se siguieron los siguientes pasos:
```
sudo apt install apache2
```
Puede verificar que ya tenga instalado Apache con el siguiente comando:
```
apache2 -v
```
Para crear el certificado occupamos OpenSSL, deberia de estar instalado por defecto en las distro mas communes, pero si no lo esta se puede instalar con el siguiente comando:
```
sudo apt install openssl
```

### Crear el certificado autofirmado

Antes de crear el certificado occupamos ejecutar esos commandos que permiten abilitar ssl y reiniar apache2:
```
sudo a2enmod ssl
sudo systemctl restart apache2
```

Para crear el certificado autofirmado se siguieron los siguientes pasos:
```
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/ssl/private/apache-selfsigned.key -out /etc/ssl/certs/apache-selfsigned.crt
```
Encontre este commando en una variedad de lugares, pero solo tras investigar un poco encontre que hace cada parte del comando. A continuacion se explica algunas de las partes del comando:
* **-days 365**: Especifica la cantidad de dias que el certificado sera valido.
* **-x509**: Especifica que el certificado sera autofirmado.
* **-keyout /etc/ssl/private/apache-selfsigned.key**: Especifica la ruta donde se guardara la llave privada.
* **-out /etc/ssl/certs/apache-selfsigned.crt**: Especifica la ruta donde se guardara el certificado.

### Para probar el certificado
Para probar el certificado se creo una pagina web basica en html y se creo un virtual host para que el servidor apache sirva la pagina web con el certificado autofirmado. Para crear el virtual host se siguieron los siguientes pasos:
1. Crear un archivo de configuracion para el virtual host en la carpeta /etc/apache2/sites-available/ con el siguiente comando:
```
sudo vim  /etc/apache2/sites-available/my_IP.conf
```
2. Dentro del archivo de configuracion se agrego lo siguiente:
```
<VirtualHost *:443>
    ServerName my_IP
    DocumentRoot /var/www/my_IP

    SSLEngine on
    SSLCertificateFile /etc/ssl/certs/apache-selfsigned.crt
    SSLCertificateKeyFile /etc/ssl/private/apache-selfsigned.key
</VirtualHost>
```

*Note:*  /var/www/my_IP es la ruta donde se encuentra la pagina web en html, usualmente /var/www/ es la ruta por defecto donde se guardan las paginas web en html en los servidores web.

3. Crear la carpeta donde se guardara la pagina web en html con el siguiente comando:
```
sudo mkdir /var/www/my_IP
```
4. Crear la pagina web en html con el siguiente comando:
```html
sudo vim /var/www/my_IP/index.html
```
5. Dentro de la pagina web en html se agrego lo siguiente:
```html
<h1>hello world</h1>
```
6. Habilitar el virtual host con el siguiente comando:
```
sudo a2ensite my_IP.conf
```

*Note:*  a2ensite es un escripto que habilita la pagina dada por argumento.

7. Reiniciar el servidor apache con el siguiente comando:
```
sudo apache2ctl configtest
sudo systemctl reload apache2
```

8. Agregar lo siguiente al archivo .conf del virtual host:
```
<VirtualHost *:80>
    ServerName my_IP
    Redirect / https://my_IP/
</VirtualHost>
```

9. Reiniciar el servidor apache con el siguiente comando:
```
sudo apachectl configtest
sudo systemctl reload apache2
```

### Resultado
El resultado de la prueba se puede ver en la siguiente imagen:
![Resultado](./cerficate_details.png)


## Referencias
* [How To Create a Self Signed SSL Certificate for Apache in Ubuntu 20.04](https://www.youtube.com/watch?v=I_x2y2GeO8w)

* [man a2ensite](https://manpages.ubuntu.com/manpages/focal/en/man8/a2ensite.8.html)

* [Tanenbaum, Computer Networks, 5th edition](https://csc-knu.github.io/sys-prog/books/Andrew%20S.%20Tanenbaum%20-%20Computer%20Networks.pdf)


