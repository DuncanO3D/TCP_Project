#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

struct Client
{
public:
	Client(SOCKET Socket, sockaddr_in Addr);
	~Client();

	SOCKET m_Socket;
	sockaddr_in m_Addr;

	char * GetName();
};
