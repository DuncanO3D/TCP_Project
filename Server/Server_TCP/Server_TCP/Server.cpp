#include "Server.h"
#include "Sockets.h"

Server::Server()
{
	m_ClientsMap = new std::map<SOCKET, sockaddr_in>();
}

Server::~Server()
{
	StopServer();
	delete m_ClientsMap;
}

void Server::LaunchServer(unsigned int Port)
{
	InitServer(Port);

	ListenClients();
}

void Server::StopServer()
{
	CloseServer();
}

std::vector<std::string> Server::GetConnectedClients()
{
	std::vector<std::string> ClientsAdress = std::vector<std::string>();

	auto it = m_ClientsMap->begin();
	std::string ClientAddress;
	unsigned short ClientPort;
	while (it != m_ClientsMap->end())
	{
		ClientAddress = Sockets::GetAdress(it->second);
		ClientPort = ntohs(it->second.sin_port);
		ClientsAdress.push_back(ClientAddress + ":" + std::to_string(ClientPort));
		it++;
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



void Server::ListenClients()
{
	do
	{
		sockaddr_in ClientAddr = { 0 };
		SOCKET newClient;
		int Lenght = sizeof(ClientAddr);

		newClient = accept(m_Socket, (sockaddr*)&ClientAddr, &Lenght);

		if (newClient != SOCKET_ERROR)
		{
			AcceptClient(newClient, ClientAddr);
		}
		else
		{
			std::cout << "Socket accept Error : " << Sockets::GetError() << std::endl;
			return;
		}
	} while (true);
}

void Server::AcceptClient(SOCKET ClientSocket, sockaddr_in ClientAddr)
{
	if (m_ClientsMap->find(ClientSocket) != m_ClientsMap->end())
		return;

	std::string ClientAddress = Sockets::GetAdress(ClientAddr);
	unsigned short ClientPort = ntohs(ClientAddr.sin_port);
	std::cout << "[" << ClientAddress << ":" << ClientPort << "]Connected" << std::endl;

	m_ClientsMap->emplace(ClientSocket, ClientAddr);


	std::cout << "Connected Clients" << std::endl;
	std::vector<std::string> ClientsStr = GetConnectedClients();
	for (unsigned int i = 0; i < ClientsStr.size(); i++)
	{
		std::cout << ClientsStr.at(i) << std::endl;
	}

	ManageClient(ClientSocket, ClientAddr, this);
}

void Server::ManageClient(SOCKET ClientSocket, sockaddr_in ClientAddr, Server * ThisServer)
{
	std::thread([ClientSocket, ClientAddr, ThisServer]()
	{
		std::string ClientAddress = Sockets::GetAdress(ClientAddr);
		unsigned short ClientPort = ntohs(ClientAddr.sin_port);

		do
		{
			char Buffer[255] = "";
			int BytesReceived = recv(ClientSocket, Buffer, 255, 0);
			if (BytesReceived <= 0)
			{
				std::cout << "[" << ClientAddress << ":" << ClientPort << "]Disconnected" << std::endl;
				ThisServer->CloseClient(ClientSocket);
				return;
			}
			else if (Buffer[0] != strlen(Buffer + 1))
			{
				std::cout << "[" << ClientAddress << ":" << ClientPort << "]Error" << std::endl;
			}
			else
			{
				std::cout << "[" << ClientAddress << ":" << ClientPort << "]" << Buffer + 1 << std::endl;

				//ThisServer->SendToClient_Thread(ClientSocket, ClientAddr, "T'es mauvais jack", ThisServer);
				//ThisServer->Broadcast(Buffer + 1);
				ThisServer->BroadCast_Thread(Buffer + 1, ThisServer);
			}
		} while (true);

	}).detach();
}

void Server::CloseClient(SOCKET ClientSocket)
{
	m_ClientsMap->erase(ClientSocket);
}

void Server::SendToClient_Thread(SOCKET ClientSocket, sockaddr_in ClientAddr, const char* Data, Server * ThisServer)
{
	std::thread([ClientSocket, ClientAddr, Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->SendToClient(ClientSocket, ClientAddr, Data);
	}).detach();
}

void Server::BroadCast_Thread(const char * Data, Server* ThisServer)
{
	std::thread([Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->Broadcast(cpyDate);
	}).detach();
}

void Server::SendToClient(SOCKET ClientSocket, sockaddr_in ClientAddr, const char * Data)
{
	std::string ClientAddress = Sockets::GetAdress(ClientAddr);
	unsigned short ClientPort = ntohs(ClientAddr.sin_port);

	char* NetworkData = Sockets::NetworkDataMaker(Data);
	int DataSize = strlen(NetworkData);

	if (send(ClientSocket, NetworkData, DataSize, 0) == DataSize)
		std::cout << "to [" << ClientAddress << ":" << ClientPort << "]" << NetworkData + 1 << std::endl;
	else
		std::cout << "to [" << ClientAddress << ":" << ClientPort << "]Error" << std::endl;
}

void Server::Broadcast(const char * Data)
{
	if (Data == "" || m_ClientsMap->size() == 0)
		return;

	auto It = m_ClientsMap->begin();
	for (It = m_ClientsMap->begin(); It != m_ClientsMap->end(); It++)
	{
		SendToClient(It->first, It->second, Data);
	}
}


void Server::SendJoinMessage(Client * JoiningClient)
{
	char DisconnectMessage[255] = "";
	strcpy_s(DisconnectMessage, JoiningClient->GetName());
	strcat_s(DisconnectMessage, " has join");
	BroadCast_Thread(DisconnectMessage, this);
}

void Server::SendLeftMessage(Client * LeavingClient)
{
	char DisconnectMessage[255] = "";
	strcpy_s(DisconnectMessage, LeavingClient->GetName());
	strcat_s(DisconnectMessage, " has left");
	BroadCast_Thread(DisconnectMessage, this);
}

void Server::SendShutDownMessage()
{
	BroadCast_Thread("Server shut down", this);
}