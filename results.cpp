#include "Results.h"

Results Results::loadResults(const string& filename) 
{
	ifstream results_file(filename);
	Results results;
	size_t size;
	results_file >> size;
	while (!results_file.eof() && size-- != 0) 
	{
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		results.addResult(iteration, static_cast<ResultValue>(result));
	}
	return results;
}

void Results::saveResults(const string& filename) const 
{
	ofstream results_file(filename);
	results_file << results.size();
	for (const auto& result : results) 
	{
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file.close();
}


size_t Results::getNextGotHitIteration() const 
{
	if (!results.empty() && results.front().second == gotHit) 
	{
		return results.front().first;
	}
	else return 0;
	// we use the fact that on iteration zero we cannot hit a bomb
	// to indicate that there isn't any result left with a gothit event
}
