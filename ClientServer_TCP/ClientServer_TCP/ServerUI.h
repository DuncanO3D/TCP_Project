#pragma once
#include "UI.h"

#include "TCP.h"
#include "UDP.h"

class ServerUI : public UI
{
public:
	ServerUI();
	~ServerUI();

	virtual void DrawUI() override;

private:
	unsigned int m_Port;
	TCP::Server * m_Server;
	UDP::Server * m_ServerUDP;

	void DrawEntrySpeech();
	void DrawChooseParameter();
	void LaunchServer();
	void DrawEndServer();
};

