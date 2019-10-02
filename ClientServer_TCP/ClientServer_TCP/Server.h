#pragma once

#include "Identity.h"
#include "Sockets.h"
#include "DataConvertor.h"
#include "DataTransfert.h"

#include <vector>
#include <thread>
#include <iostream>

class Server
{
public:
	Server();
	~Server();

	void LaunchServer(unsigned int Port);
	void StopServer();
	std::vector<std::string> GetConnectedClients();

private:
	Identity m_ServerIdentity;
	std::vector<Identity*> m_Clients;

	bool InitServer(unsigned int Port);

	bool InitSocket();
	bool BindAddr(unsigned int Port);
	bool SetServerToListen();

	void ListenClients();


	void AcceptClient(SOCKET ClientSocket, sockaddr_in ClientAddr);

	void ManageClient(Identity * NewClient, Server* ThisServer);

	void CloseClient(Identity * ToDisconnect);

	void Send_Thread(Identity * Source, Identity * Target, std::string Data, Server* ThisServer);

	void BroadCast_Thread(Identity * Source, std::string Data, Server* ThisServer);

	//void ToClient(sockaddr_in SourceAddr, Identity * Target, std::string Data);
	//void ToClient(Identity * Source, Identity * Target, std::string Data);
	//
	//void ServerBroadcast(std::string Data);
	//void ClientBroadcast(Identity * Source, std::string Data);

	void SendJoinMessage(Identity * JoiningClient);
	void SendLeftMessage(Identity * LeavingClient);
	void SendShutDownMessage();
};

