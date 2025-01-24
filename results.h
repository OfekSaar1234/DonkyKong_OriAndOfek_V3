#pragma once

#include <fstream>
#include <list>
using namespace std;

class Results {
public:

	enum ResultValue { gotHit, rescudedPauline, noResult, gameOver };

private:

	list<pair<size_t, ResultValue>> results; // pair: iteration, result

public:

	static Results loadResults(const string& filename);
	void saveResults(const string& filename) const;
	void addResult(size_t iteration, ResultValue result) {results.push_back({ iteration, result });}

	pair<size_t, ResultValue> popResult()
	{
		if (results.empty()) 
			return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}

	bool isFinishedBy(size_t iteration) const {return results.empty() || results.back().first <= iteration;}
	size_t getNextGotHitIteration() const;
};
