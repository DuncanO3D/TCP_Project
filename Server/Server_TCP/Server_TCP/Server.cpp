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
	SendShutDownMessage();

	auto it = m_ClientsSet->begin();
	while (it != m_ClientsSet->end())
	{
		delete it._Ptr->_Myval;
		it++;
	}
	m_ClientsSet->clear();

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
	Client * NewClient = new Client(ClientSocket, ClientAddr, "");
	
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
	std::cout << "[" << NewClient->m_Name << "]Connected" << std::endl;

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

void Server::ManageClient(Client * ClientStruct, Server * ThisServer)
{
	if(ClientStruct == nullptr)
		return;

	std::thread([ClientStruct, ThisServer]()
	{
		std::string ClientAddress = Sockets::GetAdress(ClientStruct->m_Addr);
		unsigned short ClientPort = ntohs(ClientStruct->m_Addr.sin_port);

		do
		{
			char Buffer[255] = "";
			int BytesReceived = recv(ClientStruct->m_Socket, Buffer, 255, 0);
			if (BytesReceived <= 0)
			{
				std::cout << "[" << ClientStruct->GetName() << "]Disconnected" << std::endl;
				ThisServer->CloseClient(ClientStruct);
				return;
			}
			else if (Buffer[0] != strlen(Buffer + 1))
			{
				std::cout << "[" << ClientStruct->GetName() << "]Error" << std::endl;
			}
			else
			{
				std::cout << "[" << ClientStruct->GetName() << "]" << Buffer + 1 << std::endl;

				ThisServer->BroadCast_Thread(Buffer + 1, ThisServer);
			}
		} while (true);

	}).detach();
}

void Server::CloseClient(Client * ClientStruct)
{
	if (ClientStruct == nullptr)
		return;

	auto it = m_ClientsSet->begin();
	while (it != m_ClientsSet->end())
	{
		if (it._Ptr->_Myval->m_Socket == ClientStruct->m_Socket)
		{
			m_ClientsSet->erase(it);
		}
	}

	SendLeftMessage(ClientStruct);

	delete ClientStruct;
}

void Server::SendToClient_Thread(Client * ClientStruct, const char* Data, Server * ThisServer)
{
	std::thread([ClientStruct, Data, ThisServer]()
	{
		char cpyDate[255] = "";
		strcpy_s(cpyDate, Data);
		ThisServer->SendToClient(ClientStruct, Data);
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

void Server::SendToClient(Client * ClientStruct, const char * Data)
{
	std::string ClientAddress = Sockets::GetAdress(ClientStruct->m_Addr);
	unsigned short ClientPort = ntohs(ClientStruct->m_Addr.sin_port);

	char* NetworkData = Sockets::NetworkDataMaker(Data);
	int DataSize = strlen(NetworkData);

	if (send(ClientStruct->m_Socket, NetworkData, DataSize, 0) == DataSize)
		std::cout << "to [" << ClientStruct->GetName() << "]" << NetworkData + 1 << std::endl;
	else
		std::cout << "to [" << ClientStruct->GetName() << "]Error" << std::endl;
}

void Server::Broadcast(const char * Data)
{
	auto It = m_ClientsSet->begin();
	for (It = m_ClientsSet->begin(); It != m_ClientsSet->end(); It++)
	{
		SendToClient(It._Ptr->_Myval, Data);
	}
}



void Server::SendJoinMessage(Client * JoiningClient)
{
	char DisconnectMessage[255] = "";
	if (JoiningClient->m_Name != "")
	{
		strcat_s(DisconnectMessage, JoiningClient->m_Name);
	}
	else
	{
		strcat_s(DisconnectMessage, Sockets::GetAdress(JoiningClient->m_Addr).c_str());
	}
	strcat_s(DisconnectMessage, " has join");
	BroadCast_Thread(DisconnectMessage, this);
}

void Server::SendLeftMessage(Client * LeavingClient)
{
	char DisconnectMessage[255] = "";
	if (LeavingClient->m_Name != "")
	{
		strcat_s(DisconnectMessage, LeavingClient->m_Name);
	}
	else
	{
		strcat_s(DisconnectMessage, Sockets::GetAdress(LeavingClient->m_Addr).c_str());
	}
	strcat_s(DisconnectMessage, " has left");
	BroadCast_Thread(DisconnectMessage, this);
}

void Server::SendShutDownMessage()
{
	BroadCast_Thread("Server shut down", this);
}
