#include "TCPSocket.h"

TCPSocket::TCPSocket(int Family, int Type, int Protocol)
{
	m_Family = Family;
	m_Type = Type;
	m_Protocol = Protocol;

	m_Socket = socket(m_Family, m_Type, m_Protocol);
	if (m_Socket == INVALID_SOCKET)
	{
		std::cout << "Socket Initialisation  Error : " << Sockets::GetError() << std::endl;
		return;
	}
}

TCPSocket::~TCPSocket()
{
	Sockets::CloseSocket(m_Socket);
}

bool TCPSocket::Connect(const std::string & ipaddress, unsigned short port)
{
	sockaddr_in server;

	inet_pton(m_Family, ipaddress.c_str(), &server.sin_addr.s_addr);

	server.sin_family = m_Family;
	server.sin_port = htons(port);
	return connect(m_Socket, (sockaddr*)&server, sizeof(server)) == 0;
}

bool TCPSocket::Send(const unsigned char * data, unsigned short len)
{
	unsigned short networkLen = htons(len);

	//Send the size
	bool DataLenght_Send = send(m_Socket, reinterpret_cast<const char*>(&networkLen), sizeof(networkLen), 0) == sizeof(networkLen);
	//Send the data
	bool Data_Send = send(m_Socket, reinterpret_cast<const char*>(data), len, 0) == len;
	
	return DataLenght_Send && Data_Send;
}

bool TCPSocket::Receive(std::vector<char*> buffer)
{
	//Get the size
	unsigned short expectedSize;
	int BytesReceived_Size = recv(m_Socket, reinterpret_cast<char*>(&expectedSize), sizeof(expectedSize), 0);
	//We verify is the receive is ok
	if (BytesReceived_Size <= 0 || BytesReceived_Size != sizeof(unsigned short))
	{
		std::cout << "Error in size received" << std::endl;
		buffer.clear();
		return false;
	}

	//ushort to TCP network byte order
	expectedSize = ntohs(expectedSize);
	buffer.resize(expectedSize);
	int receivedSize = 0;
	do
	{
		int BytesReceived_Data = recv(m_Socket, reinterpret_cast<char*>(&buffer[receivedSize]), (expectedSize - receivedSize) + sizeof(unsigned char), 0);
		if (BytesReceived_Data <= 0)
		{
			std::cout << "Error in data received" << std::endl;
			buffer.clear();
			return false;
		}
		else
		{
			receivedSize += BytesReceived_Data;
		}
	} 
	while (receivedSize < expectedSize);

	return true;
}
