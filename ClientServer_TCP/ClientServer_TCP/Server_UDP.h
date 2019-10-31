#pragma once

#include "Identity.h"
#include "Sockets.h"
#include "DataConvertor.h"
#include "DataTransfert.h"

#include <vector>
#include <thread>
#include <iostream>

namespace UDP
{
	class Server
	{
	public:
		Server();
		~Server();

		void LaunchServer(unsigned int Port);
		void StopServer();

	private:
		Network::Identity m_ServerIdentity;
		std::vector<Network::Identity*> m_Clients;

		bool m_On;

		bool InitServer(unsigned int Port);

		bool InitSocket();
		bool BindAddr(unsigned int Port);
		bool SetServerToListen();

		void ListenClients();
	};
}
