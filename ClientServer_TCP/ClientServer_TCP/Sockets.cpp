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
		char Buffer[INET6_ADDRSTRLEN] = { 0 };
		return inet_ntop(addr.sin_family, (void*)&addr.sin_addr, Buffer, INET6_ADDRSTRLEN);
	}	

	std::string GetName(sockaddr_in SourceAddr)
	{
		std::string Name;

		Name.append(Sockets::GetAdress(SourceAddr));
		Name.push_back(NETWORK_NAME_SEPARATOR);

		char Port[10] = "";
		_itoa_s(SourceAddr.sin_port, Port, 10);

		Name.append(Port);

		return Name;
	}
}