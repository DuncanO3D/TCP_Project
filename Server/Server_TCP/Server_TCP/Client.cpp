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
	char Name[255] = "";
	strcat_s(Name, Sockets::GetAdress(m_Addr).c_str());
	char Port[10] = "";
	_itoa_s(m_Addr.sin_port, Port, 10);
	strcat_s(Name, Port);
	return Name;
}