#pragma once

#include "Sockets.h"

namespace DataConvertor
{
	std::string NetworkDataMaker(sockaddr_in SourceAddr, std::string Buffer);
	std::string NetworkDataMaker(std::string Name, std::string Buffer);

	std::vector<std::string> GetDatasFromNetworkData(std::string NetworkData);
}