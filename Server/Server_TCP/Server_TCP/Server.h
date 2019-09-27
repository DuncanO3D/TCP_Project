#pragma once

#include <iostream>
#include <vector>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

struct Client
{
	SOCKET m_socket;
	sockaddr_in m_addr;
};

class Server
{
public:
	Server();
	~Server();

	bool LauncheServer(unsigned int Port);
	
	void AcceptClients();
	void CloseServer();

private:
	SOCKET m_Socket;
	sockaddr_in m_Addr;

	std::vector<Client> m_Clients;

	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();

	bool AcceptClient(sockaddr_in& ClientAddr, SOCKET& ClientSocket);

	void LauncheThreadClient(SOCKET newClient, sockaddr_in ClientAddr);

	//static for the lambda uses
	static bool ClientInteraction(SOCKET newClient, sockaddr_in ClientAddr, std::string ClientAddress, unsigned short ClientPort);
};

