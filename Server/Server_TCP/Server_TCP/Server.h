#pragma once

#include <iostream>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	bool LauncheServer(unsigned int Port);
	bool AcceptClient(sockaddr_in& ClientAddr);
	void CloseServer();

private:
	SOCKET m_Socket;
	sockaddr_in m_Addr;

	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();
};

