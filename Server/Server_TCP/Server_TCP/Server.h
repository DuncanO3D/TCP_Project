#pragma once

#include <iostream>
#include <string>

#include <vector>
#include <map>
#include <set>

#include <thread>
#include <mutex>

#include "Client.h"

//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>

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
	
	std::set<Client*> * m_ClientsSet;

	bool InitServer(unsigned int Port);
	void CloseServer();
	   
	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();

	void ListenClients();


	void AcceptClient(SOCKET ClientSocket, sockaddr_in ClientAddr);

	void ManageClient(Client * NewClient, Server* ThisServer);

	void CloseClient(Client * ToDisconnect);

	void ToClient_Thread(sockaddr_in SourceAddr, Client * Target, std::string Data, Server* ThisServer);
	void ToClient_Thread(Client * Source, Client * Target, std::string Data, Server* ThisServer);

	void ServerBroadCast_Thread(std::string Data, Server* ThisServer);
	void ClientBroadCast_Thread(Client * Source, std::string Data, Server* ThisServer);

	void ToClient(sockaddr_in SourceAddr, Client * Target, std::string Data);
	void ToClient(Client * Source, Client * Target, std::string Data);

	void ServerBroadcast(std::string Data);
	void ClientBroadcast(Client * Source, std::string Data);


	void SendJoinMessage(Client * JoiningClient);
	void SendLeftMessage(Client * LeavingClient);
	void SendShutDownMessage();
};

