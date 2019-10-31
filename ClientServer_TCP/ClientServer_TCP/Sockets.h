#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>
#include <vector>

#define NETWORK_TOTAL_SIZE 255
#define NETWORK_LENGHT_SIZE 1
#define NETWORK_SOURCE_NAME_SIZE 20
#define NETWORK_DATA_SIZE 234
#define NETWORK_NAME_SEPARATOR ':'
#define NETWORK_DATA_SEPARATOR '|'
#define LOCAL_HOST "127.0.0.1"

#define DATA_SIZE 0
#define DATA_SOURCE 1
#define DATA_BUFFER 2


namespace Sockets
{
	bool Start();
	void Release();
	int GetError();
	void CloseSocket(SOCKET socket);
	std::string GetAdress(const sockaddr_in& addr);
	std::string GetName(sockaddr_in SourceAddr);
}