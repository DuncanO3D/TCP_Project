#pragma once
#include "UI.h"

class StartupUI : public UI
{
public:
	StartupUI();
	~StartupUI();

	virtual void DrawUI() override;

private:
	void DrawEntrySpeech();
	void DrawOptions();
	int SelectOptions();
	void ManageSelectedOption(int Option);
};

