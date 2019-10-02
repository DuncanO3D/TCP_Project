#include "Client.h"

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

	m_ClientIdentity.m_Name = LocalName;
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
	if (m_Connected)
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
	m_ServerIdentity.m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ServerIdentity.m_Socket == INVALID_SOCKET)
	{
		std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
		return false;
	}

	return true;
}

bool Client::ConnectToServer(const std::string& IP, unsigned short Port)
{
	inet_pton(AF_INET, IP.c_str(), &m_ServerIdentity.m_Addr.sin_addr.s_addr);
	m_ServerIdentity.m_Addr.sin_family = AF_INET;
	m_ServerIdentity.m_Addr.sin_port = htons(Port);

	if (connect(m_ServerIdentity.m_Socket, (sockaddr*)&m_ServerIdentity.m_Addr, sizeof(m_ServerIdentity.m_Addr)))
	{
		std::cout << "Connection failed : " << Sockets::GetError() << std::endl;
		return false;
	}

	std::cout << "Connected : [" << m_ServerIdentity.GetName() << "]" << std::endl;

	return true;
}

void Client::StopClient()
{
	Sockets::CloseSocket(m_ServerIdentity.m_Socket);
	Sockets::Release();
}

void Client::ToServer_Thread(std::string Data, Client* ThisClient)
{
	std::thread * SendingThread = new std::thread([Data, ThisClient]()
	{
		DataTransfert::SendToTarget(&ThisClient->m_ClientIdentity, &ThisClient->m_ServerIdentity, Data);
	});

	m_LaunchedThreads.push_back(SendingThread);
	SendingThread->detach();
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
		int BytesReceived = recv(m_ServerIdentity.m_Socket, Buffer, 255, 0);
		if (BytesReceived <= 0)
		{
			Disconnected();
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
			std::cout << "[" << Datas[DATA_SOURCE] << "]" << Datas[DATA_BUFFER] << std::endl;
		}
	} while (true);
}

void Client::Disconnected()
{
	std::cout << "Disconnected : [" << m_ServerIdentity.GetName() << "]" << std::endl;
	m_Connected = false;
}

void Client::FreeLaunchedTreads()
{
	for (unsigned int i = 0; i < m_LaunchedThreads.size(); i++)
	{
		if (m_LaunchedThreads.at(i) != nullptr)
			delete m_LaunchedThreads.at(i);
	}
	m_LaunchedThreads.clear();
}