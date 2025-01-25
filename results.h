#pragma once

#include <fstream>
#include <list>
using namespace std;

class Results {
public:

	enum ResultValue { gotHit, rescudedPauline, noResult, gameOver };

private:

	list<pair<size_t, ResultValue>> _results; // pair: iteration, result
	size_t _score = 0;


public:

	static Results loadResults(const string& filename);
	void saveResults(const string& filename) const;
	void addResult(size_t iteration, ResultValue result) {_results.push_back({ iteration, result });}
	void addScore(size_t score) { _score = score; }
	size_t getScore() const { return _score; }

	pair<size_t, ResultValue> popResult()
	{
		if (_results.empty()) 
			return { 0, Results::noResult };
		auto result = _results.front();
		_results.pop_front();
		return result;
	}

	bool isFinishedBy(size_t iteration) const {return _results.empty() || _results.back().first <= iteration;}
	size_t getNextGotHitIteration() const;
};
