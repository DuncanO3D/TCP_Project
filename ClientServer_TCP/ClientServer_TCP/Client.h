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
	class Client
	{
	public:
		Client();
		~Client();

		bool ConnectClient(std::string IP, unsigned short Port, std::string LocalName);
		void CloseClient();

		void SendToServer(std::string data);
		void SetToListen();
		bool IsConnected();

	private:
		Network::Identity m_ClientIdentity;
		Network::Identity m_ServerIdentity;
		bool m_Connected;

		std::vector<std::thread*> m_LaunchedThreads;

		bool InitClient();
		bool InitSocket();
		bool ConnectToServer(const std::string& IP, unsigned short Port);

		void StopClient();

		void ToServer_Thread(std::string Data, Client* ThisClient);

		void LaunchListenThread(Client* ThisClient);
		void Listen();

		void Disconnected();

		void FreeLaunchedTreads();
	};
}