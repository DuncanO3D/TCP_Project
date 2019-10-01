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

	char * GetNetworkData(const char * SourceName, const char * Buffer)
	{
		char Separator = '.';
		size_t SourceLen = strlen(SourceName);
		size_t BufferLen = strlen(Buffer);

		char Lenght = SourceLen + BufferLen + 1;
		char * NetworkData = new char[255];

		//Put Size
		strcat_s(NetworkData, 1, &Lenght);
		//Put Source Name
		strcat_s(NetworkData, SourceLen, SourceName);
		//Put Separator
		strcat_s(NetworkData, 1, &Separator);
		//Put Buffer
		strcat_s(NetworkData, BufferLen, Buffer);

		return NetworkData;
	}

	std::vector<char*> GetClientDatas(const char * Buffer)
	{
		char Separator = '.';
		char* UsableBuffer = char();
		strcat_s(UsableBuffer, strlen(Buffer), Buffer);

		std::vector<char*> Datas = std::vector<char*>();

		char * Size = char();
		char * SourceName = char();
		char * DataBuffer = char();

		//Get Size
		strcat_s(Size, 1, &UsableBuffer[0]);


		char* temp;
		temp = strtok_s(UsableBuffer + 1, &Separator);


		Datas.push_back(Size);
		Datas.push_back(SourceName);
		Datas.push_back(DataBuffer);

		return Datas;
	}
}
