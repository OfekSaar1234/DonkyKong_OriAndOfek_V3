#pragma once
#include "game.h"




class AutomaticGame: public Game
{
private:

	bool _isSilent = false;




public:
	AutomaticGame(bool isSilent) : _isSilent(isSilent) {}

	void run() override {};
	void menu() override {};
};

