#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>

namespace TCP
{
	struct Identity
	{
	public:
		Identity();
		Identity(SOCKET Socket, sockaddr_in Addr);
		Identity(SOCKET Socket, sockaddr_in Addr, std::string Name);
		~Identity();

		SOCKET m_Socket;
		sockaddr_in m_Addr;
		std::string m_Name;

		std::string GetName();
	};
}