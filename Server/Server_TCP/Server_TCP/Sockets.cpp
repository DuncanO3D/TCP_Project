#include "Sockets.h"

namespace  Sockets
{
	bool Start()
	{
		WSAData wsaData;
		return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
	}

	void Release()
	{
		WSACleanup();
	}

	int GetError()
	{
		return WSAGetLastError();
	}

	void  CloseSocket(SOCKET socket)
	{
		closesocket(socket);
	}

	std::string GetAdress(const sockaddr_in & addr)
	{
		//Size of a ipv6 for both use (ipv4 and ipv6)
		char buff[INET6_ADDRSTRLEN] = { 0 };

		return inet_ntop(addr.sin_family, (void*)&addr.sin_addr, buff, INET6_ADDRSTRLEN);
	}
}