#include <iostream>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define TCP_SOCKET IPPROTO_TCP

int main()
{
	//For start networking
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != SOCKET_ERROR)
	{
		//Socket creation
		SOCKET m_ClientSocket = socket(IPV4, TCP, TCP_SOCKET);
		if (m_ClientSocket != INVALID_SOCKET)
		{
			//Parameter Server
			sockaddr_in m_ServerAdress;					   
			m_ServerAdress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			m_ServerAdress.sin_family = IPV4;
			//hton == Host to Network
			m_ServerAdress.sin_port = htons(6666);
			

			//Connection
			if (connect(m_ClientSocket, (sockaddr*) &m_ServerAdress, sizeof(m_ServerAdress)) != SOCKET_ERROR)
			{
				std::cout << "Connection to server Done" << std::endl;
			}
			else
				std::cout << "Connection to server Error" << std::endl;

			if (closesocket(m_ClientSocket) == SOCKET_ERROR)
				std::cout << "Close Socket Error" << std::endl;
		}
		else
			std::cout << "Open Socket Error" << std::endl;

		closesocket(m_ClientSocket);
	}
	else
		std::cout << "Startup Error == " << WSAGetLastError() << std::endl;
	
	//For end networking
	if (WSACleanup() == SOCKET_ERROR)
		std::cout << "Cleanup Error == " << WSAGetLastError() << std::endl;

	system("PAUSE");
}
