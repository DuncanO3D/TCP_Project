#pragma once

#include <iostream>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define TCP_SOCKET IPPROTO_TCP

class TCPSocket;

class Client
{
public:
	Client();
	~Client();

	void StartClient(int Family, int Type, int Protocol, const char* IP, unsigned int Port);
	void StopClient();

private:
	TCPSocket * m_Socket = nullptr;
};

