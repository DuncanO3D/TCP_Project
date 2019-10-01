#include "Client.h"
#include "Sockets.h"

Client::Client(SOCKET Socket, sockaddr_in Addr, const char * Name)
{
	m_Socket = Socket;
	m_Addr = Addr;
	strcpy_s(m_Name, Name);
}

Client::~Client()
{
}

char * Client::GetName()
{
	if (m_Name != "")
		return m_Name;
	else
		Sockets::GetAdress(m_Addr);
}

bool Client::operator==(const Client a)
{
	return m_Socket == a.m_Socket;
}
