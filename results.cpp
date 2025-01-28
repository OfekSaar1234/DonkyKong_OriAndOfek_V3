#include "Results.h"

Results Results::loadResults(const string& filename) 
{
	Results results;
	ifstream results_file(filename);
	if (!results_file.is_open())
		results._doesExist = false;
	else
	{
		size_t size;
		size_t score;

		results_file >> size;
		while (!results_file.eof() && size-- != 0)
		{
			size_t iteration;
			int result;
			results_file >> iteration >> result;
			results.addResult(iteration, static_cast<ResultValue>(result));
		}
		results_file >> score;
		results.addScore(score);
	}
	results_file.close();
	return results;
}

void Results::saveResults(const string& filename) const 
{
	ofstream results_file(filename);
	results_file << _results.size();
	for (const auto& result : _results) 
	{
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file << '\n' << _score;
	results_file.close();
}


size_t Results::getNextGotHitIteration() const
{
	if (!_results.empty() && _results.front().second == gotHit) 
	{
		return _results.front().first;
	}
	else return 0;
}
