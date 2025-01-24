#include <fstream>

#include "Steps.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	ifstream steps_file(filename);
	steps_file >> steps.randomSeed;
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	return steps;
}

void Steps::saveSteps(const string& filename) const {
	ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}

char Steps::popStep(size_t iteration)
{
	char step = steps.front().second;
	steps.pop_front();
	return step;
}
