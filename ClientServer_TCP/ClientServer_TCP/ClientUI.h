#pragma once
#include "UI.h"

#include "TCP.h"

class ClientUI : public UI
{
public:
	ClientUI();
	~ClientUI();

	virtual void DrawUI() override;

private:
	TCP::Client * m_Client;

	std::string m_IP;
	unsigned int m_Port;
	std::string m_Name;

	void DrawEntrySpeech();
	void DrawChooseParameter();
	bool ConnectToServer();
	void Chat();
	void DrawEndServer();
};

