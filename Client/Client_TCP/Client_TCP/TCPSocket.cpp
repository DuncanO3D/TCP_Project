#include "TCPSocket.h"
#include <iostream>

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
