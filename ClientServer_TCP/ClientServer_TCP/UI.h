#pragma once

#include <iostream>

class UI
{
public:
	UI();
	~UI();

	virtual void Launch();
	virtual void Stop();

protected:
	bool m_On;
	UI * m_Child;

	bool TryLaunchChild();
	void TrySafeDestroyChild();
	virtual void DrawUI() = 0;
};

