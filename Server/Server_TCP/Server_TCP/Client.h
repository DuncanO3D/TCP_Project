#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

struct Client
{
public:
	Client(SOCKET Socket, sockaddr_in Addr, const char* Name);
	~Client();

	SOCKET m_Socket;
	sockaddr_in m_Addr;
	char m_Name[255];

	char * GetName();

	bool operator==(const Client a);
};

