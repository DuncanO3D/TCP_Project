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

char * Client::GetName()
{
	return Sockets::GetName(m_Addr);
}