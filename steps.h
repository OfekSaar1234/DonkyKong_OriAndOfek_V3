#pragma once

#include <list>
using namespace std;

class Steps {

	long randomSeed = 0;
	list<pair<size_t, char>> steps; // pair: iteration, step

public:

	static Steps loadSteps(const string& filename);
	void saveSteps(const string& filename) const;
	long getRandomSeed() const { return randomSeed; }
	void setRandomSeed(long seed) { randomSeed = seed; }
	void addStep(size_t iteration, char step) { steps.push_back({ iteration, step }); }
	bool isNextStepOnIteration(size_t iteration) const { return !steps.empty() && steps.front().first == iteration; }
	char popStep(size_t iteration);
};
