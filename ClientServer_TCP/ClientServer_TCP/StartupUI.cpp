#include "StartupUI.h"

#include "ServerUI.h"
#include "ClientUI.h"

StartupUI::StartupUI() : UI()
{
}

StartupUI::~StartupUI()
{
}

void StartupUI::DrawUI()
{
	DrawEntrySpeech();
	DrawOptions();
	int Option = SelectOptions();
	ManageSelectedOption(Option);
}

void StartupUI::DrawEntrySpeech()
{
	std::cout << "---Welcome to SygChat---" << std::endl;
	std::cout << "SygChat is a TCP IRC type chat" << std::endl << std::endl;
}

void StartupUI::DrawOptions()
{
	std::cout << "Select a option :" << std::endl;
	std::cout << "1 : Launch server" << std::endl;
	std::cout << "2 : Launch client" << std::endl;
	std::cout << "3 : Quit application" << std::endl;
}

int StartupUI::SelectOptions()
{
	bool CorrectOption = false;
	int Option = 0;
	do
	{
		std::cin >> Option;

		if (Option == 1 || Option == 2 || Option == 3)
			CorrectOption = true;
		else
			std::cout << "Please choose a correct option" << std::endl;

	} while (!CorrectOption);
	   
	return Option;
}

void StartupUI::ManageSelectedOption(int Option)
{
	switch (Option)
	{
	case 1:
		m_Child = new ServerUI();
		TryLaunchChild();
		TrySafeDestroyChild();
		break;

	case 2:
		m_Child = new ClientUI();
		TryLaunchChild();
		TrySafeDestroyChild();
		break;

	case 3:
		Stop();
		break;

	default:
		break;
	}
}
