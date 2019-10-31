#pragma once

#include "Identity.h"
#include "Sockets.h"
#include "DataConvertor.h"
#include "DataTransfert.h"

#include <vector>
#include <thread>
#include <iostream>

namespace TCP
{
	class Server
	{
	public:
		Server();
		~Server();

		void LaunchServer(unsigned int Port);
		void StopServer();
		std::vector<std::string> GetConnectedClients();

	private:
		Network::Identity m_ServerIdentity;
		std::vector<Network::Identity*> m_Clients;

		bool m_On;

		bool InitServer(unsigned int Port);

		bool InitSocket();
		bool BindAddr(unsigned int Port);
		bool SetServerToListen();

		void ListenClients();


		void AcceptClient(SOCKET ClientSocket, sockaddr_in ClientAddr);

		void ManageClient(Network::Identity * NewClient, Server* ThisServer);

		void CloseClient(Network::Identity * ToDisconnect);

		void Send_Thread(Network::Identity * Source, Network::Identity * Target, std::string Data, Server* ThisServer);

		void BroadCast_Thread(Network::Identity * Source, std::string Data, Server* ThisServer);

		void SendJoinMessage(Network::Identity * JoiningClient);
		void SendLeftMessage(Network::Identity * LeavingClient);
		void SendShutDownMessage();
	};
}