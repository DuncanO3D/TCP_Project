#pragma once

#include <iostream>
#include <vector>
#include <map>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

struct Client
{
	Client(SOCKET sock, sockaddr_in addr) { m_socket = sock; m_addr = addr; }
	SOCKET m_socket;
	sockaddr_in m_addr;

	bool operator==(Client b) { return m_socket == b.m_socket && m_addr.sin_addr.s_addr == b.m_addr.sin_addr.s_addr; }
};

class Server
{
public:
	Server();
	~Server();
	
	void LaunchServer(unsigned int Port);
	void StopServer();
	std::vector<std::string> GetConnectedClients();

private:
	SOCKET m_Socket;
	sockaddr_in m_Addr;
	
	std::vector<Client>* m_Clients;

	bool InitServer(unsigned int Port);
	void CloseServer();
	   
	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();

	void AcceptClients();

	void AddClient(Client newClient);
	void RemoveClient(Client newClient);

	bool AcceptClient(sockaddr_in& ClientAddr, SOCKET& ClientSocket);

	void LauncheThreadClient(SOCKET newClient, sockaddr_in ClientAddr, Server* ThisServer);

	bool ClientInteraction(SOCKET newClient, sockaddr_in ClientAddr, std::string ClientAddress, unsigned short ClientPort);
};

