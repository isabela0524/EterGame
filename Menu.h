#pragma once

#include <iostream>
#include <string>
#include "Match.h"
#include "GamemodeDistributor.h"
#include "Gamemode.h"

import Game_UI;

class Menu
{
private:
	bool askBooleanOption(const std::string& question);
	Gamemode showMenu();
public:
	void handleMatch();
};

