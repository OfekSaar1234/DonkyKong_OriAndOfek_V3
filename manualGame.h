#pragma once
#include "game.h"



class ManualGame : public Game
{
private:

	bool _isSave = false; 

	void run() override;

public:
	ManualGame(bool isSave) : _isSave(isSave) {}

	void menu() override; // Runs the Manual Game menu
};

