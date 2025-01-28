#include <fstream>

#include "Steps.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	ifstream steps_file(filename);
	if (!steps_file.is_open())
		steps._doesExist = false;
	else
	{
		steps_file >> steps._colorMode;
		steps_file >> steps._randomSeed;
		size_t size;
		steps_file >> size;
		while (!steps_file.eof() && size-- != 0) {
			size_t iteration;
			char step;
			steps_file >> iteration >> step;
			steps.addStep(iteration, step);
		}
	}
	steps_file.close();
	return steps;
}

void Steps::saveSteps(const string& filename) const {
	ofstream steps_file(filename);
	steps_file << _colorMode << '\n' << _randomSeed << '\n' << _steps.size();
	for (const auto& step : _steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
}

char Steps::popStep()
{
	char step = _steps.front().second;
	_steps.pop_front();
	return step;
}
