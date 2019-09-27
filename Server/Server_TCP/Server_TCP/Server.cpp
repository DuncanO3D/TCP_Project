#include "Server.h"
#include "Sockets.h"

#include <thread>
#include <mutex>


Server::Server()
{
	m_Clients = new std::vector<Client>();
}

Server::~Server()
{
	StopServer();
	delete m_Clients;
}

void Server::LaunchServer(unsigned int Port)
{
	InitServer(Port);

	AcceptClients();
}

void Server::StopServer()
{
	CloseServer();
}

std::vector<std::string> Server::GetConnectedClients()
{
	std::vector<std::string> ClientsAdress = std::vector<std::string>();

	for (int i = 0; i < m_Clients->size(); i++)
	{
		ClientsAdress.push_back(Sockets::GetAdress(m_Clients->at(i).m_addr));
	}

	return ClientsAdress;
}



bool Server::InitServer(unsigned int Port)
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

void Server::AcceptClients()
{
	//While whe have no error
	do
	{
		sockaddr_in ClientAddr = { 0 };
		SOCKET newClient;
		if (AcceptClient(ClientAddr, newClient))
		{
			//Thread/Client
			
			LauncheThreadClient(newClient, ClientAddr, this);
		}
		else
		{
			std::cout << "Socket accept Error : " << Sockets::GetError() << std::endl;
			break;
		}

	} while (true);
}

void Server::AddClient(Client newClient)
{
	for (unsigned int i = 0; i < m_Clients.size(); i++)
	{
		if (m_Clients[i] == newClient)
		{
			return;
		}
	}
	m_Clients.push_back(newClient);
}

void Server::RemoveClient(Client newClient)
{
	unsigned int Index = 0;
	for (unsigned int i = 0; i < m_Clients.size(); i++)
	{
		if (m_Clients[i] == newClient)
		{
			m_Clients.erase(m_Clients.begin() + Index);
			return;
		}
		else
			Index++;
	}
}

bool Server::AcceptClient(sockaddr_in& ClientAddr, SOCKET& ClientSocket)
{
	ClientAddr = { 0 };
	int Lenght = sizeof(ClientAddr);

	ClientSocket = accept(m_Socket, (sockaddr*)&ClientAddr, &Lenght);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "Socket accept Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}


void Server::LauncheThreadClient(SOCKET newClient, sockaddr_in ClientAddr, Server* ThisServer)
{
	//Add Mutex if we need a control of thread order
	std::thread([newClient, ClientAddr, ThisServer]()
	{
		std::string ClientAddress = Sockets::GetAdress(ClientAddr);
		unsigned short ClientPort = ntohs(ClientAddr.sin_port);

		std::cout << "Connection [" << ClientAddress << ":" << ClientPort << "]" << std::endl;
		bool connected = true;
		
		ThisServer->AddClient(Client(newClient, ClientAddr));

		//While we have the connection with the client
		do
		{
			//Do the interaction with the client (ex : recv and send)

			ThisServer->ClientInteraction(newClient, ClientAddr, ClientAddress, ClientPort);

		} while (true);

		std::cout << "Deconnection [" << ClientAddress << ":" << ClientPort << "]" << std::endl;

		ThisServer->RemoveClient(Client(newClient, ClientAddr));

	}).detach();
}

bool Server::ClientInteraction(SOCKET newClient, sockaddr_in ClientAddr, std::string ClientAddress, unsigned short ClientPort)
{
	char Buffer[200] = { 0 };

	int BytesReceived = recv(newClient, Buffer, 199, 0);
	if (BytesReceived == 0 || BytesReceived == SOCKET_ERROR)
		return false;

	std::cout << "[" << ClientAddress << ":" << ClientPort << "]" << Buffer << std::endl;

	int BytesSend = send(newClient, Buffer, BytesReceived, 0);
	if (BytesSend == 0 || BytesSend == SOCKET_ERROR)
		return false;

	return true;
}
