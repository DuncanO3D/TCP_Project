#include "Server.h"
#include "Sockets.h"
#include "LogSystem.h"

Server::Server()
{
}

Server::~Server()
{
}

bool Server::LauncheServer(unsigned int Port)
{
	//Winsock Init
	if (Sockets::Start() == false)
	{
		std::cout << "Winsock Initialisation Error : " << Sockets::GetError() << std::endl;
		return false;
	}

	//Socket Init
	if (InitSocket() == false)
		return false;

	//Addr Binding
	if (BindAddr(Port) == false)
		return false;

	//Set Server to listen
	if (SetServerToListen() == false)
		return false;

	std::cout << "Server Launched on port " << Port << std::endl;

	return true;
}

bool Server::AcceptClient(sockaddr_in& ClientAddr)
{
	ClientAddr = { 0 };
	int Lenght = sizeof(ClientAddr);

	SOCKET newClient = accept(m_Socket, (sockaddr*)&ClientAddr, &Lenght);
	if (newClient == INVALID_SOCKET)
	{
		std::cout << "Socket accept Error : " << Sockets::GetError() << std::endl;
		return false;
	}

	std::cout << "Connection : " << Sockets::GetAdress(ClientAddr) << " on port " << ClientAddr.sin_port << std::endl;

	return true;
}

void Server::CloseServer()
{
	Sockets::CloseSocket(m_Socket);
	Sockets::Release();
}


bool Server::InitSocket()
{
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}

bool Server::BindAddr(unsigned int Port)
{
	//Addr Binding
	m_Addr.sin_addr.s_addr = INADDR_ANY; // All source accepted
	m_Addr.sin_port = htons(Port);
	m_Addr.sin_family = AF_INET; //TCP

	int error = bind(m_Socket, (sockaddr*)&m_Addr, sizeof(m_Addr));
	if (error != 0)
	{
		std::cout << "Addr bind Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}

bool Server::SetServerToListen()
{
	int error = listen(m_Socket, SOMAXCONN); //System Max Connection
	if (error != 0)
	{
		std::cout << "Server listen Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}
