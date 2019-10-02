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

bool Client::ConnectClient(std::string IP, unsigned short Port, std::string LocalName)
{
	if (InitClient() == false)
		return false;

	//Connect to server
	if (ConnectToServer(IP, Port) == false)
		return false;

	m_Name = LocalName;
	m_Connected = true;
}

void Client::CloseClient()
{
	FreeLaunchedTreads();
	StopClient();
	m_Connected = false;
}

void Client::SendToServer(std::string data)
{
	if(m_Connected)
		ToServer_Thread(data, this);
}

void Client::SetToListen()
{
	if (m_Connected)
		LaunchListenThread(this);
}

bool Client::IsConnected()
{
	return m_Connected;
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

void Client::ToServer_Thread(std::string Data, Client* ThisClient)
{
	std::thread * SendingThread = new std::thread([Data, ThisClient]()
	{
		ThisClient->ToServer(Data);
	});

	m_LaunchedThreads.push_back(SendingThread);
	SendingThread->detach();
}
void Client::ToServer(std::string Data)
{
	std::string NetworkData = Sockets::NetworkDataMaker(m_Name, Data);

	if (send(m_Socket, NetworkData.c_str(), NetworkData.size(), 0) != NetworkData.size())
	{
		std::cout << "Error Size" << std::endl;
	}
}

void Client::LaunchListenThread(Client* ThisClient)
{
	std::thread * ListenThread = new std::thread([ThisClient]()
	{		
		ThisClient->Listen();
	});

	m_LaunchedThreads.push_back(ListenThread);
	ListenThread->detach();
}
void Client::Listen()
{
	do 
	{
		char Buffer[255] = "";
		int BytesReceived = recv(m_Socket, Buffer, 255, 0);
		if (BytesReceived <= 0)
		{
			Disconnected();
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
			std::cout << "[" << Datas[DATA_SOURCE] << "]" << Datas[DATA_BUFFER] << std::endl;
		}
	} while (true);
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