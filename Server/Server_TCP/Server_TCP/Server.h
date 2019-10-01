#pragma once

#include <iostream>
#include <string>

#include <vector>
#include <map>
#include <set>

#include <thread>
#include <mutex>

#include "Client.h"

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

	void ManageClient(Client * ClientStruct, Server* ThisServer);

	void CloseClient(Client * ClientStruct);

	void SendToClient_Thread(Client * ClientStruct, const char* Data, Server* ThisServer);

	void BroadCast_Thread(const char * Data, Server* ThisServer);

	void SendToClient(Client * ClientStruct, const char* Data);

	void Broadcast(const char * Data);



	void SendJoinMessage(Client * JoiningClient);
	void SendLeftMessage(Client * LeavingClient);
	void SendShutDownMessage();
};

