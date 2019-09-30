#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
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
	
	void LaunchServer(unsigned int Port);
	void StopServer();
	std::vector<std::string> GetConnectedClients();

private:
	SOCKET m_Socket;
	sockaddr_in m_Addr;
	
	//std::vector<SOCKET>* m_Clients;
	std::map<SOCKET, sockaddr_in>* m_ClientsMap;


	bool InitServer(unsigned int Port);
	void CloseServer();
	   
	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();

	void ListenClients();


	void AcceptClient(SOCKET ClientSocket, sockaddr_in ClientAddr);

	void ManageClient(SOCKET ClientSocket, sockaddr_in ClientAddr, Server* ThisServer);

	void CloseClient(SOCKET ClientSocket);

	void SendToClient(SOCKET ClientSocket, sockaddr_in ClientAddr, const char* Data, Server* ThisServer);

	//To do
	void BradCast();
};

