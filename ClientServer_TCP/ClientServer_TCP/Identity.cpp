#include "Identity.h"

#include "Sockets.h"

namespace Network
{
	Identity::Identity()
	{
	}

	Identity::Identity(SOCKET Socket, sockaddr_in Addr)
	{
		m_Socket = Socket;
		m_Addr = Addr;
		m_Name = "";
	}

	Identity::Identity(SOCKET Socket, sockaddr_in Addr, std::string Name)
	{
		m_Socket = Socket;
		m_Addr = Addr;
		m_Name = Name;
	}

	Identity::~Identity()
	{
	}

	std::string Identity::GetName()
	{
		if (m_Name == "")
			return Sockets::GetName(m_Addr);
		else
			return m_Name;
	}
}