#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <winsock2.h>
#include <iostream>
#include <string>

void initWinsock();
SOCKET createSocket(int addressFamily, int type, int protocol);
void connectToServer(SOCKET socket, const struct sockaddr* server, int addrlen);
void sendData(SOCKET socket,const char* message, int messageLen);
int receiveData(SOCKET socket, char* reply, int len);
void closeConnection(SOCKET socket);
void setSocketOption(SOCKET socket, int level, int optname, const char *optval, int optlen);

#endif // SOCKETCLIENT_H
