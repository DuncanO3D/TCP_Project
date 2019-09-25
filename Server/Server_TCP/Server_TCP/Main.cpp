#include <iostream>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define TCP_SOCKET IPPROTO_TCP
#define ANY_SOURCE INADDR_ANY

int main()
{
	//For start networking
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != SOCKET_ERROR)
	{
		//Socket creation
		SOCKET m_ServerSocket = socket(IPV4, TCP, TCP_SOCKET);
		if (m_ServerSocket != INVALID_SOCKET)
		{
			const unsigned short m_Port = 9999;

			sockaddr_in m_ServerAdress;
			m_ServerAdress.sin_addr.S_un.S_addr = ANY_SOURCE;
			// 0 for autimaticaly choose a open PORT
			m_ServerAdress.sin_port = htons(m_Port);
			m_ServerAdress.sin_family = IPV4;

			if (bind(m_ServerSocket, (struct sockaddr *) &m_ServerAdress, sizeof(m_ServerAdress)) != SOCKET_ERROR)
			{
				if (listen(m_ServerSocket, SOMAXCONN) != SOCKET_ERROR)
				{
					std::cout << "Serveur launched on port : " << m_Port << std::endl;

					sockaddr_in m_ConnectionAddress = { 0 };
					int len = sizeof(m_ConnectionAddress);

					if (SOCKET newClient = accept(m_ServerSocket, (SOCKADDR*)& m_ConnectionAddress, &len) != INVALID_SOCKET)
					{
						char Buffer[INET6_ADDRSTRLEN];
						//Convert IPV4 to String
						std::string clientAddress = inet_ntop(m_ServerAdress.sin_family, (void*)&(m_ServerAdress.sin_addr), Buffer, INET6_ADDRSTRLEN);
						std::cout << "Connexion de " << clientAddress.c_str() << ":" << m_ServerAdress.sin_port << std::endl;
					}
					else
						std::cout << "Client Socket Error" << std::endl;
				}
				else
					std::cout << "Listen Error" << std::endl;
			}
			else
				std::cout << "Adress Binding Error" << std::endl;
		}
		else
			std::cout << "Open Socket Error" << std::endl;

		closesocket(m_ServerSocket);
	}
	else
		std::cout << "Startup Error == " << WSAGetLastError() << std::endl;

	//For end networking
	if (WSACleanup() == SOCKET_ERROR)
		std::cout << "Cleanup Error == " << WSAGetLastError() << std::endl;

	system("PAUSE");
}