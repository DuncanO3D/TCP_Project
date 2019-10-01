#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <iostream>

namespace  Sockets
{
	bool Start();
	void Release();
	int GetError();
	void CloseSocket(SOCKET socket);
	std::string GetAdress(const sockaddr_in& addr);

	char * GetNetworkData(const char * SourceName, const char * Buffer);
	std::vector<char*> GetClientDatas(const char * Buffer);
};

