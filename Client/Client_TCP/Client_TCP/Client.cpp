#include "Client.h"
#include "Sockets.h"

#include <thread>
#include <mutex>

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
	StopClient();
	m_Connected = false;
}

void Client::SendToServer(const char * data, unsigned short len)
{
	if(m_Connected)
		Send(reinterpret_cast<const unsigned char *>(data), len);
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

	return true;
}

void Client::StopClient()
{
	Sockets::CloseSocket(m_Socket);
	Sockets::Release();
}


void Client::Send(const unsigned char * data, unsigned short len)
{
	unsigned short networkLen = htons(len);

	LaunchSendingThread(data, networkLen, len, this);
}

void Client::Receive(std::vector<char*> buffer)
{
	LaunchListenThread(this);
}



void Client::LaunchSendingThread(const unsigned char * data, unsigned short networklen, unsigned short len, Client* ThisClient)
{
	std::thread([data, networklen, len, ThisClient]()
	{
		//Send the size
		bool DataLenght_Send = send(ThisClient->m_Socket, reinterpret_cast<const char*>(&networklen), sizeof(networklen), 0) == sizeof(networklen);
		//Send the data
		bool Data_Send = send(ThisClient->m_Socket, reinterpret_cast<const char*>(data), len, 0) == len;

		if (DataLenght_Send && Data_Send)
			ThisClient->SendingDone(data);
		else
			ThisClient->SendingError(data);
	}).detach();
}

void Client::SendingDone(const unsigned char * DataSend)
{
	std::cout << DataSend << " : Send" << std::endl;
}
void Client::SendingError(const unsigned char * DataSend)
{
	std::cout << DataSend << " : Send Error" << std::endl;
}


void Client::LaunchListenThread(Client* ThisClient)
{
	std::thread([ThisClient]()
	{
		std::vector<char*> buffer;
		unsigned short expectedSize;

		int BytesReceived_Size = recv(ThisClient->m_Socket, reinterpret_cast<char*>(&expectedSize), sizeof(expectedSize), 0);
		if (!(BytesReceived_Size <= 0 || BytesReceived_Size != sizeof(unsigned short)))
		{
			ThisClient->ListenError("Error in size received");
			return;
		}

		expectedSize = ntohs(expectedSize);
		buffer.resize(expectedSize);
		int receivedSize = 0;

		do
		{
			int BytesReceived_Data = recv(ThisClient->m_Socket, reinterpret_cast<char*>(&buffer[receivedSize]), (expectedSize - receivedSize) + sizeof(unsigned char), 0);
			if (BytesReceived_Data <= 0)
			{
				ThisClient->ListenError("Error in size received");
				return;
			}
			else
			{
				receivedSize += BytesReceived_Data;
			}
		} while (receivedSize < expectedSize);

		ThisClient->ListenDone(buffer);
	}).detach();
}

void Client::ListenDone(std::vector<char*> DataBuffer)
{
	for (unsigned i = 0; i < DataBuffer.size(); i++)
	{
		std::cout << DataBuffer.at(i) << " : Received" << std::endl;
	}
}
void Client::ListenError(const char* Error)
{
	std::cout << "Error : " << Error << std::endl;
}