#include "Server.h"
#include "Sockets.h"

Server::Server()
{
}

Server::~Server()
{
	StopServer();
}

void Server::LaunchServer(unsigned int Port)
{
	InitServer(Port);

	ListenClients();
}

void Server::StopServer()
{
	Sockets::CloseSocket(m_ServerIdentity.m_Socket);
	Sockets::Release();

	for (int i = 0; i < m_Clients.size(); i++)
	{
		delete m_Clients[i];
	}
	m_Clients.clear();
}

std::vector<std::string> Server::GetConnectedClients()
{
	std::vector<std::string> ClientsAdress = std::vector<std::string>();

	for (int i = 0; i < m_Clients.size(); i++)
	{
		ClientsAdress.push_back(m_Clients[i]->GetName());
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

bool Server::InitSocket()
{
	m_ServerIdentity.m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ServerIdentity.m_Socket == INVALID_SOCKET)
	{
		std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}

bool Server::BindAddr(unsigned int Port)
{
	//Addr Binding
	m_ServerIdentity.m_Addr.sin_addr.s_addr = 0;
	m_ServerIdentity.m_Addr.sin_port = htons(Port);
	m_ServerIdentity.m_Addr.sin_family = AF_INET; //TCP

	int error = bind(m_ServerIdentity.m_Socket, (sockaddr*)&m_ServerIdentity.m_Addr, sizeof(m_ServerIdentity.m_Addr));
	if (error != 0)
	{
		std::cout << "Addr bind Error : " << Sockets::GetError() << std::endl;
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

		newClient = accept(m_ServerIdentity.m_Socket, (sockaddr*)&ClientAddr, &Lenght);

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
	Identity * NewClient = new Identity();
	NewClient->m_Socket = ClientSocket;
	NewClient->m_Addr = ClientAddr;


	for (int i = 0; i < m_Clients.size(); i++)
	{
		if (m_Clients[i]->m_Socket == NewClient->m_Socket)
		{
			delete NewClient;
			return;
		}
	}

	std::cout << "[" << NewClient->GetName() << "]Connected" << std::endl;

	m_Clients.push_back(NewClient);

	SendJoinMessage(NewClient);

	ManageClient(NewClient, this);
}


void Server::ManageClient(Identity * NewClient, Server * ThisServer)
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

			std::vector<std::string> Datas = DataConvertor::GetDatasFromNetworkData(Buffer);
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

				ThisServer->BroadCast_Thread(NewClient, Datas[DATA_BUFFER], ThisServer);
			}
		} while (true);
	}).detach();
}

void Server::CloseClient(Identity * ToDisconnect)
{
	for (int i = 0; i < m_Clients.size(); i++)
	{
		if (m_Clients[i]->m_Socket == ToDisconnect->m_Socket)
		{
			m_Clients.erase(m_Clients.begin() + i);
			break;
		}
	}

	SendLeftMessage(ToDisconnect);

	delete ToDisconnect;
}


void Server::Send_Thread(Identity * Source, Identity * Target, std::string Data, Server * ThisServer)
{
	std::thread([Source, Target, Data, ThisServer]()
	{
		DataTransfert::SendToTarget(Source, Target, Data);
	}).detach();
}

void Server::BroadCast_Thread(Identity * Source, std::string Data, Server * ThisServer)
{
	std::thread([Source, Data, ThisServer]()
	{
		DataTransfert::Broadcast(Source, ThisServer->m_Clients, Data);
	}).detach();
}


void Server::SendJoinMessage(Identity * JoiningClient)
{
	std::string JoiningMessage = JoiningClient->GetName();
	JoiningMessage.append(" has join");
	BroadCast_Thread(&m_ServerIdentity, JoiningMessage, this);
}
void Server::SendLeftMessage(Identity * LeavingClient)
{
	std::string LeavingMessage = LeavingClient->GetName();
	LeavingMessage.append(" has left");
	BroadCast_Thread(&m_ServerIdentity, LeavingMessage, this);
}
void Server::SendShutDownMessage()
{
	BroadCast_Thread(&m_ServerIdentity, "Server shut down", this);
}