#include "Server.h"
#include "Sockets.h"

Server::Server()
{
	m_ClientsSet = new std::set<Client*>();
}

Server::~Server()
{
	StopServer();
	delete m_ClientsSet;
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

	auto it = m_ClientsSet->begin();
	while (it != m_ClientsSet->end())
	{
		ClientsAdress.push_back(it._Ptr->_Myval->GetName());
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
	Client * NewClient = new Client(ClientSocket, ClientAddr);

	auto it = m_ClientsSet->begin();
	while (it != m_ClientsSet->end())
	{
		if (it._Ptr->_Myval->m_Socket == NewClient->m_Socket)
		{
			delete NewClient;
			return;
		}
	}

	std::string ClientAddress = Sockets::GetAdress(ClientAddr);
	unsigned short ClientPort = ntohs(ClientAddr.sin_port);
	std::cout << "[" << NewClient->GetName() << "]Connected" << std::endl;

	auto pair = m_ClientsSet->emplace(NewClient);


	//std::cout << "Connected Clients" << std::endl;
	//std::vector<std::string> ClientsStr = GetConnectedClients();
	//for (unsigned int i = 0; i < ClientsStr.size(); i++)
	//{
	//	std::cout << ClientsStr.at(i) << std::endl;
	//}

	SendJoinMessage(NewClient);

	ManageClient(pair.first._Ptr->_Myval, this);
}

void Server::ManageClient(Client * NewClient, Server * ThisServer)
{
	std::thread([NewClient, ThisServer]()
	{
		std::string ClientAddress = Sockets::GetAdress(NewClient->m_Addr);
		unsigned short ClientPort = ntohs(NewClient->m_Addr.sin_port);

		do
		{
			char Buffer[255] = "";
			int BytesReceived = recv(NewClient->m_Socket, Buffer, 255, 0);
			if (BytesReceived <= 0)
			{
				std::cout << "[" << ClientAddress << ":" << ClientPort << "]Disconnected" << std::endl;
				ThisServer->CloseClient(NewClient);
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
				ThisServer->ClientBroadCast_Thread(NewClient, Buffer + 1, ThisServer);
			}
		} while (true);
	}).detach();
}

void Server::CloseClient(Client * ToDisconnect)
{
	if (ToDisconnect == nullptr)
		return;

	auto it = m_ClientsSet->begin();
	while (it != m_ClientsSet->end())
	{
		if (it._Ptr->_Myval->m_Socket == ToDisconnect->m_Socket)
		{
			m_ClientsSet->erase(it);
		}
	}

	SendLeftMessage(ToDisconnect);

	delete ToDisconnect;
}

void Server::ToClient_Thread(SOCKET SourceSocket, sockaddr_in SourceAddr, Client * Target, const char * Data, Server * ThisServer)
{
	std::thread([SourceSocket, SourceAddr, Target, Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->ToClient(SourceSocket, SourceAddr, Target, Data);
	}).detach();
}
void Server::ToClient_Thread(Client * Source, Client * Target, const char * Data, Server * ThisServer)
{
	std::thread([Source, Target, Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->ToClient(Source, Target, Data);
	}).detach();
}

void Server::ServerBroadCast_Thread(const char * Data, Server* ThisServer)
{
	std::thread([Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->ServerBroadcast(cpyDate);
	}).detach();
}
void Server::ClientBroadCast_Thread(Client * Source, const char * Data, Server * ThisServer)
{
	std::thread([Source, Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->ClientBroadcast(Source, cpyDate);
	}).detach();
}

void Server::ToClient(SOCKET SourceSocket, sockaddr_in SourceAddr, Client * Target, const char* Data)
{
	char* NetworkData = Sockets::NetworkDataMaker(SourceSocket, SourceAddr, Data);
	int DataSize = strlen(NetworkData);
	   
	if (send(Target->m_Socket, NetworkData, DataSize, 0) == DataSize)
		std::cout << "to [" << Target->GetName() << "]" << NetworkData + 1 << std::endl;
	else
		std::cout << "to [" << Target->GetName() << "]Error" << std::endl;
}
void Server::ToClient(Client * Source, Client * Target, const char * Data)
{
	ToClient(Source->m_Socket, Source->m_Addr, Target, Data);
}

void Server::ServerBroadcast(const char * Data)
{
	if (Data == "" || m_ClientsSet->size() == 0)
		return;

	auto It = m_ClientsSet->begin();
	for (It = m_ClientsSet->begin(); It != m_ClientsSet->end(); It++)
	{
		ToClient(m_Socket, m_Addr, It._Ptr->_Myval, Data);
	}
}
void Server::ClientBroadcast(Client * Source, const char * Data)
{
	if (Data == "" || m_ClientsSet->size() == 0)
		return;

	auto It = m_ClientsSet->begin();
	for (It = m_ClientsSet->begin(); It != m_ClientsSet->end(); It++)
	{
		if(Source->m_Socket != It._Ptr->_Myval->m_Socket)
			ToClient(Source, It._Ptr->_Myval, Data);
	}
}


void Server::SendJoinMessage(Client * JoiningClient)
{
	char DisconnectMessage[255] = "";
	strcpy_s(DisconnectMessage, JoiningClient->GetName());
	strcat_s(DisconnectMessage, " has join");
	ServerBroadCast_Thread(DisconnectMessage, this);
}
void Server::SendLeftMessage(Client * LeavingClient)
{
	char DisconnectMessage[255] = "";
	strcpy_s(DisconnectMessage, LeavingClient->GetName());
	strcat_s(DisconnectMessage, " has left");
	ServerBroadCast_Thread(DisconnectMessage, this);
}
void Server::SendShutDownMessage()
{
	ServerBroadCast_Thread("Server shut down", this);
}