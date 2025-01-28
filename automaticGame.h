#pragma once
#include "game.h"
#include "steps.h"
#include "results.h"



class AutomaticGame: public Game
{
private:

	bool _isSilent = false;
	bool _isValid = true;

	virtual void run() override;
	void reportResultError(const string& message, const string& filename, size_t iteration);
	void loadValidGame();
	void loadInValidGame();

public:
	AutomaticGame(bool isSilent) : _isSilent(isSilent), _isValid(true) {}
	virtual ~AutomaticGame() = default;

	virtual void menu() override;
};

