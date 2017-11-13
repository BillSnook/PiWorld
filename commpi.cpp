#include "commpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


commPi::commPi()
{

    connected = false;
}

bool commPi::getCommStateConnected() {

    return connected;
}

bool commPi::connectTo( const char *target )
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    connected = false;
    portno = 5555;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr,"ERROR opening socket\n");
        return false;
    }
    server = gethostbyname( target );
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return false;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    int didConnect = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if ( didConnect < 0 ) {
        fprintf(stderr,"ERROR connecting, %d\n", didConnect);
        return false;
    }
    connected = true;
    return true;
}

char *commPi::sendMessage( const char *message )
{
    char *buff;
    long n = 0;

    if ( ! connected ) {
        fprintf(stderr,"ERROR sending message while not connected\n");
        return 0;
    }

    n = write(sockfd,message,strlen(message));
    if (n < 0) {
        fprintf(stderr,"ERROR writing to socket\n");
        connected = false;
        return 0;
    }

    buff = (char *)malloc( 256 );
    bzero(buff,256);
    n = read(sockfd,buff,255);
    if (n < 0) {
        fprintf(stderr,"ERROR reading from socket\n");
        connected = false;
        return 0;
    }
    return buff;
}

bool commPi::detachFrom()
{
    close(sockfd);
    connected = false;

    return true;
}


