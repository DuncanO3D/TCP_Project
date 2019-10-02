#include "Client.h"
#include "Sockets.h"

Client::Client(SOCKET Socket, sockaddr_in Addr)
{
	m_Socket = Socket;
	m_Addr = Addr;
}

Client::~Client()
{
}

std::string Client::GetName()
{
	if (m_Name == "")
		return Sockets::GetName(m_Addr);
	else
		return m_Name;
}
