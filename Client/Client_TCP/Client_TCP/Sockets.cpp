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


	std::string NetworkDataMaker(sockaddr_in SourceAddr, std::string Buffer)
	{
		std::string Name = GetName(SourceAddr);
		char Lenght = Buffer.size() + Name.size() + 2;

		std::string NetworkData;

		NetworkData.push_back(Lenght);
		NetworkData.append(Name);
		NetworkData.push_back(NETWORK_DATA_SEPARATOR);
		NetworkData.append(Buffer);

		return NetworkData;
	}
	std::string NetworkDataMaker(std::string Name, std::string Buffer)
	{
		char Lenght = Buffer.size() + Name.size() + 2;

		std::string NetworkData;

		NetworkData.push_back(Lenght);
		NetworkData.append(Name);
		NetworkData.push_back(NETWORK_DATA_SEPARATOR);
		NetworkData.append(Buffer);

		return NetworkData;
	}


	std::vector<std::string> GetDatasFromNetworkData(std::string NetworkData)
	{
		if (NetworkData.size() < 3)
			return std::vector<std::string>();

		std::vector<std::string> Datas;
		std::string Lenght;
		std::string Name;
		std::string Data;

		//Get Lenght
		Lenght.push_back(NetworkData[0]);

		//Get Name
		int DataSeparatorIndex = NetworkData.find(NETWORK_DATA_SEPARATOR);
		Name.append(NetworkData.substr(1, DataSeparatorIndex - 1));

		//Get Data
		Data.append(NetworkData.substr(DataSeparatorIndex + 1, Datas.size() - DataSeparatorIndex + 1));

		Datas.push_back(Lenght);
		Datas.push_back(Name);
		Datas.push_back(Data);

		return Datas;
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