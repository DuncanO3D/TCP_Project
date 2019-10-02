#include "DataTransfert.h"

namespace DataTransfert
{
	int SendToTarget(Identity * Source, Identity * Target, std::string Data)
	{
		std::string NetworkData = DataConvertor::NetworkDataMaker(Source->GetName(), Data);

		return send(Target->m_Socket, NetworkData.c_str(), NetworkData.size(), 0);
	}

	void Broadcast(Identity * Source, std::vector<Identity*> m_Targets, std::string Data, bool toSource)
	{
		for (int i = 0; i < m_Targets.size(); i++)
		{
			if (toSource == false && Source->m_Socket == m_Targets[i]->m_Socket)
				continue;

			SendToTarget(Source, m_Targets[i], Data);
		}
	}
}
