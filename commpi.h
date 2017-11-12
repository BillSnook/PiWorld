#ifndef COMMPI_H
#define COMMPI_H

//#include <QObject>

class commPi //: public QObject
{

public:
    explicit commPi();

    int sockfd, portno;

    bool getCommStateConnected();
    bool connectTo( const char *target );
    char *sendMessage( const char *message );
    bool detachFrom();

private:
    bool connected;

};

#endif // COMMPI_H
