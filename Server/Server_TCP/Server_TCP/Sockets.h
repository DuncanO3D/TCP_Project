#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

namespace Sockets
{
	bool Start();
	void Release();
	int GetError();
	void CloseSocket(SOCKET socket);
	std::string GetAdress(const sockaddr_in& addr);
	char * NetworkDataMaker(const char * Buffer);
};

