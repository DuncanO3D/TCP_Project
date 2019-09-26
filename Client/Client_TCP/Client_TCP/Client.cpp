#include "Client.h"
#include "Sockets.h"
#include "TCPSocket.h"


Client::Client()
{
}


Client::~Client()
{
}

void Client::StartClient(int Family, int Type, int Protocol, const char* IP, unsigned int Port)
{	
	if (Sockets::Start() == false)
	{
		std::cout << "Initialisation Error : " << Sockets::GetError() << std::endl;
		return;
	}
	
	m_Socket = new TCPSocket(Family, Type, Protocol);
	if (!m_Socket->Connect(IP, Port))
	{
		std::cout << "Connection Error : " << Sockets::GetError() << std::endl;
		return;
	}
	std::cout << "Socket connecte !" << std::endl;
}

void Client::StopClient()
{
	Sockets::Release();
}
