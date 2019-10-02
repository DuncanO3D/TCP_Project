#pragma once

#include "Sockets.h"
#include "DataConvertor.h"
#include "Identity.h"

namespace DataTransfert
{
	int SendToTarget(Identity * Source, Identity * Target, std::string Data);

	void Broadcast(Identity * Source, std::vector<Identity*> m_Targets, std::string Data, bool toSource = true);
};

