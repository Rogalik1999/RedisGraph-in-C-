#include "SocketClient.h"
#include "Error.h"

//#pragma comment(lib, "ws2_32.lib")

void initWinsock(){

    static bool isInitialized = false;
    if(!isInitialized){
        static WSADATA wsadata;
        int err = WSAStartup(MAKEWORD(2,2), &wsadata);
        if (err != 0) {
           std::string error = "Nie udalo sie zainicjalizowac Winsock\n";
           throw SocketError(error);
        }
        isInitialized = true;
    }
}

SOCKET createSocket(int addressFamily, int type, int protocol){

    SOCKET sock;

    sock = socket(addressFamily, type, protocol);

    if(sock == INVALID_SOCKET){
         std::string error = "Nie udalo sie stworzyc socket\n";
         throw SocketError(error);
    }

    return sock;
}


void connectToServer(SOCKET socket, const struct sockaddr* server, int addrlen){

    if (connect(socket , server, addrlen) < 0)
	{
		std::string error = "Nie udalo sie polaczyc z serwerem\n";
        throw SocketError(error);
	}

}

void sendData(SOCKET socket,const char* message, int messageLen){

    if(send(socket , message , messageLen , 0) < 0)
	{
		std::string error = "Blad podczas wysylania do serwera\n";
        throw SocketError(error);
	}

}

int receiveData(SOCKET socket, char* reply, int len){

    int recv_size;
    if((recv_size = recv(socket, reply , len , 0)) == SOCKET_ERROR)
	{
		std::string error = "Blad podczas odbierania z serwera\n";
		//std::string error = std::to_string(WSAGetLastError());
        throw SocketError(error);
	}

	return recv_size;

}

void closeConnection(SOCKET socket){

    if(closesocket(socket) == SOCKET_ERROR){
        std::string error = "Nie udalo sie zamknac polaczenia\n";
        throw SocketError(error);
    }

}

void setSocketOption(SOCKET socket, int level, int optname, const char *optval, int optlen){

    if(setsockopt(socket,level,optname,optval,optlen) == SOCKET_ERROR){

        std::string error = "Nie udalo sie zmienic opcji dotyczace ogolnego programu obslugi gniazd\n";
        throw SocketError(error);

    }

}

