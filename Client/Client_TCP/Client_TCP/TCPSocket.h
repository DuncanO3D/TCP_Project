#pragma once

#include "Sockets.h"
#include <string>

class TCPSocket
{
public:
	TCPSocket(int Family, int Type, int Protocol);
	~TCPSocket();

	bool Connect(const std::string& ipaddress, unsigned short port);

private:
	int m_Family;
	int m_Type;
	int m_Protocol;
	SOCKET m_Socket;
};

