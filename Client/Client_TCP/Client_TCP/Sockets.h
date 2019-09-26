#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace  Sockets
{
	bool Start();
	void Release();
	int GetError();
	void CloseSocket(SOCKET socket);
};

