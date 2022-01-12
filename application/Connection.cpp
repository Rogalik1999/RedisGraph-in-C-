//---------------------------------------------------------------------------

#pragma hdrstop

#include "Connection.h"

#pragma package(smart_init)

Connection::Connection(int portNumber, char* host):  portNumber(portNumber), host(host), onConnect(false)
{

}


Connection::~Connection(){

	if(onConnect){
		try{
			closeConnection(sock);
		}catch(SocketError& blad){
        }
	}
}

void Connection::connect(){

    initWinsock();

	try{

        sock = createSocket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server;

        server.sin_addr.s_addr = inet_addr(host);
        server.sin_family = AF_INET;
        server.sin_port = htons(portNumber);

        connectToServer(sock,(struct sockaddr *)&server, sizeof(server));

        onConnect = true;

    }catch(SocketError& blad){
        throw SocketError(blad);
    }

}

void Connection::disconnect(){

    if(onConnect){
        try{
            closeConnection(sock);
            onConnect = false;
        }catch(SocketError& blad){

        }

    }
}

void Connection::sendCommand(const char* message){

    if(onConnect){
        try{
            sendData(sock,message,strlen(message));
        }catch(SocketError& blad){
            throw SocketError(blad);
        }

    }

}

int Connection::receiveResponse(char* reply, int len){
    if(onConnect){

        try{

            int recvSize;
            recvSize = receiveData(sock,reply,len);
            if(recvSize > 0){
                //Add a NULL terminating character to make it a proper string before printing
				//reply[recvSize] = '\0';std::cout<<"in process 4 "<<redisgraphparser.process<<std::endl;
            }
            return recvSize;

        }
        catch(SocketError& blad){
            throw SocketError(blad);
        }


    }

	return 0;
}