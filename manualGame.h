#pragma once
#include "game.h"



class ManualGame : public Game
{
private:

	bool _isSave = false; 

	void run() override;
	bool validKey(const char keyPressed)const;
	void levelsScreen(); // Displays levels for the user to choose
	void gameOverScreen(); // Displays the game over screen
	void gameWinningScreen(); // Displays the game-winning screen
	void nextLevelScreen(); // Displays the next level screen
	void gameErrorScreen(); // Displays the error screen
	void InstructionsScreen(); // Displays the instructions screen
	void MenuScreen(); // Displays the main menu

public:
	ManualGame(bool isSave) : _isSave(isSave) {}
	virtual ~ManualGame() = default;

	void menu() override; // Runs the Manual Game menu
};

