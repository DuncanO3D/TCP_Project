#pragma once

#include "Client.h"
#include "Server.h"
#include "Identity.h"
#include "Sockets.h"
#include "DataConvertor.h"
#include "DataTransfert.h"

namespace TCP
{
	bool ContainCharacter(std::string Buffer, std::string Exeption);
}