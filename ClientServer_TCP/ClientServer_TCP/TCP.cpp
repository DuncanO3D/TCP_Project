#include "TCP.h"

namespace TCP
{
	bool ContainCharacter(std::string Buffer, std::string Exeption)
	{
		for (unsigned int i = 0; i < Buffer.size(); i++)
		{
			if (Buffer[i] < 48 || Buffer[i] > 57)
			{
				bool IsExeption = false;
				for (unsigned int j = 0; j < Exeption.size(); j++)
				{
					if (Buffer[i] == Exeption[j])
					{
						IsExeption = true;
						break;
					}
				}
				if (IsExeption == false)
					return true;
			}
		}
		return false;
	}
}