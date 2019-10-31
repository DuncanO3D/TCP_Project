#pragma once

#include "Sockets.h"
#include "DataConvertor.h"
#include "Identity.h"


namespace DataTransfert
{
	int SendToTarget(Network::Identity * Source, Network::Identity * Target, std::string Data);

	void Broadcast(Network::Identity * Source, std::vector<Network::Identity*> m_Targets, std::string Data, bool toSource = true);
}