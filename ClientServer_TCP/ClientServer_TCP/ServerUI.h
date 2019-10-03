#pragma once
#include "UI.h"

#include "TCP.h"

class ServerUI : public UI
{
public:
	ServerUI();
	~ServerUI();

	virtual void DrawUI() override;

private:
	unsigned int m_Port;
	TCP::Server * m_Server;

	void DrawEntrySpeech();
	void DrawChooseParameter();
	void LaunchServer();
	void DrawEndServer();
};

