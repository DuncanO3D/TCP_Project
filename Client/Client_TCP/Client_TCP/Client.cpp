#include "Client.h"
#include "Sockets.h"

Client::Client()
{
	m_Connected = false;
}

Client::~Client()
{
	CloseClient();
}


bool Client::ConnectClient(const char * IP, unsigned short Port)
{
	if (InitClient() == false)
		return false;

	//Connect to server
	if (ConnectToServer(IP, Port) == false)
		return false;

	m_Connected = true;
}

void Client::CloseClient()
{
	FreeLaunchedTreads();
	StopClient();
	m_Connected = false;
}

void Client::SendToServer(const char * data)
{
	if(m_Connected)
		Send(data);
}

void Client::SetToListen()
{
	if (m_Connected)
		Receive(std::vector<char*>());
}


bool Client::InitClient()
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

	return true;
}

bool Client::InitSocket()
{
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET)
	{
		std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
		return false;
	}
	return true;
}

bool Client::ConnectToServer(const std::string& IP, unsigned short Port)
{
	sockaddr_in server;
	inet_pton(AF_INET, IP.c_str(), &server.sin_addr.s_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);

	if (connect(m_Socket, (sockaddr*)&server, sizeof(server)))
	{
		std::cout << "Connection failed : " << Sockets::GetError() << std::endl;
		return false;
	}
	std::cout << "Connected : [" << IP << ":" << Port << "]" << std::endl;
	m_ConnectedServer_Address = IP;
	m_ConnectedServer_Port = Port;

	return true;
}

void Client::StopClient()
{
	Sockets::CloseSocket(m_Socket);
	Sockets::Release();
}


void Client::Send(const char * data)
{
	LaunchSendingThread(data, this);
}

void Client::Receive(std::vector<char*> buffer)
{
	LaunchListenThread(this);
}



void Client::LaunchSendingThread(const char * data, Client* ThisClient)
{
	std::thread * SendingThread = new std::thread([data, ThisClient]()
	{
		char* NetworkData = Sockets::NetworkDataMaker(data);
		int DataSize = strlen(NetworkData);

		if (send(ThisClient->m_Socket, NetworkData, DataSize, 0) != DataSize)
		{
			std::cout << "Sending Error" << std::endl;
		}
	});

	m_LaunchedThreads.push_back(SendingThread);
	SendingThread->detach();
}


void Client::LaunchListenThread(Client* ThisClient)
{
	std::thread * ListenThread = new std::thread([ThisClient]()
	{		
		do
		{
			char Buffer[255] = "";
			int BytesReceived = recv(ThisClient->m_Socket, Buffer, 255, 0);
			if (BytesReceived <= 0)
			{
				ThisClient->Disconnected();
				return;
			}
			else if (Buffer[0] != strlen(Buffer + 1))
			{
				std::cout << "[" << ThisClient->m_ConnectedServer_Address << ":" << ThisClient->m_ConnectedServer_Port << "]Listen Error" << std::endl;
			}
			else
			{
				std::cout << "[" << ThisClient->m_ConnectedServer_Address << ":" << ThisClient->m_ConnectedServer_Port << "]" << Buffer + 1 << std::endl;
			}
		} while (true);

	});

	m_LaunchedThreads.push_back(ListenThread);
	ListenThread->detach();
}

void Client::Disconnected()
{
	std::cout << "Disconnected : [" << m_ConnectedServer_Address << ":" << m_ConnectedServer_Port << "]" << std::endl;
	m_ConnectedServer_Address = "";
	m_ConnectedServer_Port = 0;
	m_Connected = false;
}


void Client::FreeLaunchedTreads()
{
	for (unsigned int i = 0; i < m_LaunchedThreads.size(); i++)
	{
		if(m_LaunchedThreads.at(i) != nullptr)
			delete m_LaunchedThreads.at(i);
	}
	m_LaunchedThreads.clear();
}