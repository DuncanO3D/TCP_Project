#include "UI.h"

UI::UI()
{
	m_On = false;
	m_Child = nullptr;
}

UI::~UI()
{
	TrySafeDestroyChild();
}

void UI::Launch()
{
	m_On = true;
	system("CLS");
	do
	{
		DrawUI();
	} while (m_On);
}

void UI::Stop()
{
	system("CLS");
	m_On = false;
}

bool UI::TryLaunchChild()
{
	if (m_Child != nullptr)
	{
		m_Child->Launch();
		return true;
	}
	return false;
}

void UI::TrySafeDestroyChild()
{
	if (m_Child != nullptr)
		delete m_Child;
	m_Child = nullptr;
}
