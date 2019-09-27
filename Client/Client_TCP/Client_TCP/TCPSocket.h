#pragma once

#include "Sockets.h"
#include <iostream>
#include <string>
#include <vector>

class TCPSocket
{
public:
	TCPSocket(int Family, int Type, int Protocol);
	~TCPSocket();

	bool Connect(const std::string& ipaddress, unsigned short port);

	bool Send(const unsigned char* data, unsigned short len);
	bool Receive(std::vector<char*> buffer);

private:
	int m_Family;
	int m_Type;
	int m_Protocol;
	SOCKET m_Socket;
};

