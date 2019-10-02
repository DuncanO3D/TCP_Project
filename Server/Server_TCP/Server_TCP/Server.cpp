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
	ULONG Addr;
	inet_pton(AF_INET, LOCAL_HOST, &Addr);
	m_Addr.sin_addr.s_addr = Addr;

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

	std::cout << "[" << NewClient->GetName() << "]Connected" << std::endl;

	auto pair = m_ClientsSet->emplace(NewClient);
	
	SendJoinMessage(NewClient);

	ManageClient(pair.first._Ptr->_Myval, this);
}

void Server::ManageClient(Client * NewClient, Server * ThisServer)
{
	std::thread([NewClient, ThisServer]()
	{
		do
		{
			char Buffer[255] = "";			
			int BytesReceived = recv(NewClient->m_Socket, Buffer, 255, 0);
			if (BytesReceived <= 0)
			{
				std::cout << "[" << NewClient->GetName() << "]Disconnected" << std::endl;
				ThisServer->CloseClient(NewClient);
				return;
			}

			std::vector<std::string> Datas = Sockets::GetDatasFromNetworkData(Buffer);
			if (Datas.size() < 3)
				return;
			
			unsigned int DataSize = *Datas[DATA_SIZE].c_str();

			if (DataSize != strlen(Buffer))
			{
				std::cout << "[" << Datas[DATA_SOURCE] << "]Error Size" << std::endl;
			}
			else
			{
				NewClient->m_Name = Datas[DATA_SOURCE];
				std::cout << "[" << Datas[DATA_SOURCE] << "]" << Datas[DATA_BUFFER] << std::endl;

				ThisServer->ClientBroadCast_Thread(NewClient, Datas[DATA_BUFFER], ThisServer);
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
			break;
		}
	}

	SendLeftMessage(ToDisconnect);

	delete ToDisconnect;
}

void Server::ToClient_Thread(sockaddr_in SourceAddr, Client * Target, std::string Data, Server * ThisServer)
{
	std::thread([SourceAddr, Target, Data, ThisServer]()
	{
		ThisServer->ToClient(SourceAddr, Target, Data);
	}).detach();
}
void Server::ToClient_Thread(Client * Source, Client * Target, std::string Data, Server * ThisServer)
{
	std::thread([Source, Target, Data, ThisServer]()
	{
		ThisServer->ToClient(Source, Target, Data);
	}).detach();
}

void Server::ServerBroadCast_Thread(std::string Data, Server* ThisServer)
{
	std::thread([Data, ThisServer]()
	{
		ThisServer->ServerBroadcast(Data);
	}).detach();
}
void Server::ClientBroadCast_Thread(Client * Source, std::string Data, Server * ThisServer)
{
	std::thread([Source, Data, ThisServer]()
	{
		ThisServer->ClientBroadcast(Source, Data);
	}).detach();
}

void Server::ToClient(sockaddr_in SourceAddr, Client * Target, std::string Data)
{
	std::string NetworkData = Sockets::NetworkDataMaker(SourceAddr, Data);
	   
	if (send(Target->m_Socket, NetworkData.c_str(), NetworkData.size(), 0) == NetworkData.size())
		std::cout << "to [" << Target->GetName() << "]" << Data << std::endl;
	else
		std::cout << "to [" << Target->GetName() << "]Error Size" << std::endl;
}
void Server::ToClient(Client * Source, Client * Target, std::string Data)
{
	std::string NetworkData = Sockets::NetworkDataMaker(Source, Data);

	if (send(Target->m_Socket, NetworkData.c_str(), NetworkData.size(), 0) == NetworkData.size())
		std::cout << "to [" << Target->GetName() << "]" << Data << std::endl;
	else
		std::cout << "to [" << Target->GetName() << "]Error Size" << std::endl;
}

void Server::ServerBroadcast(std::string Data)
{
	if (Data == "" || m_ClientsSet->size() == 0)
		return;

	auto It = m_ClientsSet->begin();
	for (It = m_ClientsSet->begin(); It != m_ClientsSet->end(); It++)
	{
		ToClient(m_Addr, It._Ptr->_Myval, Data);
	}
}
void Server::ClientBroadcast(Client * Source, std::string Data)
{
	if (Data == "" || m_ClientsSet->size() == 0)
		return;

	auto It = m_ClientsSet->begin();
	for (It = m_ClientsSet->begin(); It != m_ClientsSet->end(); It++)
	{
		//if(Source->m_Socket != It._Ptr->_Myval->m_Socket)
			ToClient(Source, It._Ptr->_Myval, Data);
	}
}


void Server::SendJoinMessage(Client * JoiningClient)
{
	std::string JoiningMessage = JoiningClient->GetName();
	JoiningMessage.append(" has join");
	ServerBroadCast_Thread(JoiningMessage, this);
}
void Server::SendLeftMessage(Client * LeavingClient)
{
	std::string LeavingMessage = LeavingClient->GetName();
	LeavingMessage.append(" has left");
	ServerBroadCast_Thread(LeavingMessage, this);
}
void Server::SendShutDownMessage()
{
	ServerBroadCast_Thread("Server shut down", this);
}