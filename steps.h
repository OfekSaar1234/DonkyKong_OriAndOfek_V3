#pragma once

#include <list>
using namespace std;

class Steps {
private:
	bool _doesExist = true;
	int _colorMode = 0;
	long _randomSeed = 0;
	list<pair<size_t, char>> _steps; // pair: iteration, step

public:

	static Steps loadSteps(const string& filename);
	void saveSteps(const string& filename) const;

	bool getDoesExist() const { return _doesExist; }
	long getRandomSeed() const { return _randomSeed; }
	bool getColorMode() const { return _colorMode; }

	void setRandomSeed(long seed) { _randomSeed = seed; }
	void setColorMode(bool color) { color ? _colorMode = 1 : _colorMode = 0; }


	void addStep(size_t iteration, char step) { _steps.push_back({ iteration, step }); }
	bool isNextStepOnIteration(size_t iteration) const { return !_steps.empty() && _steps.front().first == iteration; }
	char popStep();
};
