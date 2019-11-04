#include "Server_UDP.h"

namespace UDP
{
	Server::Server()
	{
		m_On = false;
	}

	Server::~Server()
	{
		StopServer();
	}

	void Server::LaunchServer(unsigned int Port)
	{
		InitServer(Port);
		m_On = true;
		ListenClients();
	}

	void Server::StopServer()
	{
		Sockets::CloseSocket(m_ServerIdentity.m_Socket);
		Sockets::Release();

		for (unsigned int i = 0; i < m_Clients.size(); i++)
		{
			delete m_Clients[i];
		}
		m_Clients.clear();

		m_On = false;
	}

	bool Server::InitServer(unsigned int Port)
	{
		//Winsock Init
		if (Sockets::Start() == false)
		{
			std::cout << "Winsock Initialisation Error : " << Sockets::GetError() << std::endl;
			system("PAUSE");
			return false;
		}

		//Socket Init
		if (InitSocket() == false)
			return false;

		//Addr Binding
		if (BindAddr(Port) == false)
			return false;

		//Set Server to listen
		//if (SetServerToListen() == false)
		//	return false;

		std::cout << "Server Launched on port " << Port << std::endl;

		return true;
	}

	bool Server::InitSocket()
	{
		m_ServerIdentity.m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_ServerIdentity.m_Socket < 0)
		{
			std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
			system("PAUSE");
			return false;
		}
		return true;
	}

	bool Server::BindAddr(unsigned int Port)
	{
		//Addr Binding
		m_ServerIdentity.m_Addr.sin_addr.s_addr = INADDR_ANY;
		m_ServerIdentity.m_Addr.sin_port = htons(Port);
		m_ServerIdentity.m_Addr.sin_family = AF_INET; //IPV4

		int error = bind(m_ServerIdentity.m_Socket, reinterpret_cast<sockaddr*>(&m_ServerIdentity.m_Addr), sizeof(m_ServerIdentity.m_Addr));
		if (error != 0)
		{
			std::cout << "Addr bind Error : " << Sockets::GetError() << std::endl;
			system("PAUSE");
			return false;
		}

		m_ServerIdentity.m_Name = "Server";

		return true;
	}

	bool Server::SetServerToListen()
	{
		int error = listen(m_ServerIdentity.m_Socket, SOMAXCONN); //System Max Connection
		if (error != 0)
		{
			std::cout << "Server listen Error : " << Sockets::GetError() << std::endl;
			system("PAUSE");
			return false;
		}
		return true;
	}


	void Server::ListenClients()
	{
		std::cout << "Close the window to shut down the server" << std::endl;

		sockaddr_in from;
		int slen = sizeof(from);
		char Buf[512];

		do
		{
			memset(Buf, '\0', 512);

			int recv_len = recvfrom(m_ServerIdentity.m_Socket, Buf, 255, 0, reinterpret_cast<sockaddr *>(&from), &slen);
			if (recv_len <= 0)
			{
				std::cout << "Socket listen Error : " << Sockets::GetError() << std::endl;
				system("PAUSE");
				break;
			}

			std::cout << std::string(Buf) << std::endl;
					   			
			int send_len = sendto(m_ServerIdentity.m_Socket, Buf, 255, 0, reinterpret_cast<sockaddr *>(&from), slen);
			if (send_len <= 0)
			{
				std::cout << "Socket listen Error : " << Sockets::GetError() << std::endl;
				system("PAUSE");
				break;
			}
		} while (true);

		StopServer();
	}
}