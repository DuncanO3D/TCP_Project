#include "DataConvertor.h"

namespace DataConvertor
{
	std::string NetworkDataMaker(sockaddr_in SourceAddr, std::string Buffer)
	{
		std::string Name = Sockets::GetName(SourceAddr);
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
}