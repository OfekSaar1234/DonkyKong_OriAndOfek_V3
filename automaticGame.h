#pragma once
#include "game.h"
#include "steps.h"
#include "results.h"



class AutomaticGame: public Game
{
private:

	bool _isSilent = false;

	void run() override;
	void reportResultError(const std::string& message, const std::string& filename, size_t iteration);
	void loadValidScreen();

public:
	AutomaticGame(bool isSilent) : _isSilent(isSilent) {}
	virtual ~AutomaticGame() = default;

	void menu() override;
};

