#ifndef CONNECTION_H
#define CONNECTION_H

#include <winsock2.h>
#include "SocketClient.h"
#include "Error.h"
#include "RedisGraphParser.h"

class Connection
{

    int portNumber;
    char* host;
    bool onConnect;


public:
    SOCKET sock;
    Connection(int portNumber, char* host);
    ~Connection();
    void connect();
    void disconnect();
    void sendCommand(const char* message);
    int receiveResponse(char* reply, int len);

};

#endif // CONNECTION_H
