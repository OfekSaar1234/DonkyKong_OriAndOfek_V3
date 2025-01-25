#include "automaticGame.h"


void AutomaticGame::menu() 
{
	ShowConsoleCursor(false); // Hide the console cursor
	getAllMapsFileNames(); // get all files name into the vector 
	sort(_fileNames.begin(), _fileNames.end());  // Sort the vector in ascending order

	run();
}


void AutomaticGame::run()
{
	
	clrscr(); // Clear screen
	Mario mario; // Create Mario character

	bool unmatching_result_found = false;

	if (_fileNames.empty()) // If no maps are available
	{
		noMapsErrorScreen(); // Display error message
	}

	// Loop through each stage in the game
	for (size_t stage = _levelNum; stage < _fileNames.size(); stage++)
	{
		clrscr(); // Clear screen for the new stage
		Map map; // Create a new map object

		long random_seed = 0;
		size_t iteration = 0;
		size_t score = 0;

		Steps steps;
		Results results;

		string filename_prefix = _fileNames[stage].substr(0, _fileNames[stage].find_last_of('.'));
		string stepsFilename = filename_prefix + ".steps";
		string resultsFilename = filename_prefix + ".result";



		if (map.load(_fileNames[stage])) // If map successfully loads
		{
			steps = Steps::loadSteps(stepsFilename);
			random_seed = steps.getRandomSeed();
			results = Results::loadResults(resultsFilename);

			srand(random_seed);

			map.reset();   // Reset map to initial state
			map.enableColors(steps.getColorMode()); // Enable or disable colors based on user input
			map.print(/*_isSilent*/);   // Print the map

			mario.resetToNextStage(map.getMarioPosition()); // Reset Mario's position for the new stage

			map.drawLife(map.getLPosition(), mario.getLife()); // Draw Mario's life count on the map
			map.drawScore(map.getLPosition(), _score); // Draw score on the map

			vector<Enemy*> enemies; // containert for all the enemies in the stage

			Hammer hammer; // Create a Hammer object
			hammer.setMap(map); // Set map for the hammer

			mario.setMap(map);   // Link Mario to the current map
			mario.setHammer(hammer); // Link hammer to Mario

			Point barrelPosition = map.getDkongPosition(); // Get Donkey Kong's position

			// Set the barrel starting position depending on Donkey Kong's location
			if (map.getCharOriginalMap({ barrelPosition.getX() - 1, barrelPosition.getY() }) == (char)GameConfig::utilKeys::EDGE)
			{
				barrelPosition.setX(barrelPosition.getX() + 1); // Barrel starts from the right
			}
			else
			{
				barrelPosition.setX(barrelPosition.getX() - 1); // Barrel starts from the left
			}

			// Initialize ghosts
			vector<Point> ghostPos = map.getGhostPositions();

			// Initialize smart ghosts
			vector<Point> smartGhostsPos = map.getSmartGhostPositions();

			for (const auto& pos : ghostPos)
			{
				Ghost* ghost = new Ghost(pos);
				ghost->setMap(map);
				enemies.push_back(ghost);
			}

			for (const auto& pos : smartGhostsPos)
			{
				SmartGhost* smartGhost = new SmartGhost(pos);
				smartGhost->setMap(map);
				enemies.push_back(smartGhost);
			}

			// Game loop runs while Mario has life and hasn't rescued Pauline
			while (mario.getLife() > 0 && !mario.rescuedPauline())
			{
				size_t nextGotHitIteration = 0;

				if (results.isFinishedBy(iteration)) {
					reportResultError("Results file finished while game hadn't!", _fileNames[stage], iteration);
					unmatching_result_found = true;
					break;
				}
				else 
				{
					nextGotHitIteration = results.getNextGotHitIteration();
				}

				// use steps loaded from file
				if (steps.isNextStepOnIteration(iteration)) 
				{
					mario.keyPressed(steps.popStep());
				}

				// Draw active barrels and ghosts
				for (const auto& enemy : enemies)
				{
					if (!enemy->isDestroyed()) // if this enemy isn't destroyed
					{
						enemy->draw();
					}
				}

				mario.draw(); // Draw Mario

				Sleep(_isSilent ? 0 : GameConfig::MOVE_DELAY_LOAD);

				// handles enemy's actions
				for (size_t i = 0; i < enemies.size();)
				{
					Enemy* enemy = enemies[i];

					// Check if the enemy is a barrel, if it's a barrel, cast the pointer to Barrel*, else return nullptr
					Barrel* barrel = dynamic_cast<Barrel*>(enemy);

					if (barrel && barrel->HitEdge()) // if barrel hits the edge
					{
						barrel->erase();// Erase barrel from screen
						delete barrel;// Free memory
						enemies.erase(enemies.begin() + i); // Remove from vector
					}
					else if (enemy->gotHit()) // if enemy got hit by mario's hammer
					{
						enemy->erase(); // erases enemy from screen

						if (barrel) // if enemy is a Barrel
						{
							delete enemy;// Free memory
							enemies.erase(enemies.begin() + i); // Remove from vector
						}
						else // enemy is a Ghost / SmartGhost
						{
							enemy->setStatus(true);
						}

						score += 1;
						_score += 1;// Increase score
						_scoreChange = true; // Mark score for redrawing
						++i; // Continue to the next enemy
					}
					else if (barrel && barrel->isDestroyed()) // if it's a barrel if it got exploded
					{
						barrel->clearExplosion();
						delete barrel;// Free memory
						enemies.erase(enemies.begin() + i); // Remove from vector
					}
					else
					{
						enemy->erase();// Erase for movement effect
						++i;// Continue to the next enemy
					}
				}

				mario.erase(); // Erase Mario from the map

				// Move enemy
				for (const auto& enemy : enemies)
				{
					if (!enemy->isDestroyed()) // if this enemy is a ghost
					{
						enemy->move();
					}
				}

				mario.move(); // Move Mario

				_gameLoop += 1; // Increment game loop counter

				if (_gameLoop >= GameConfig::LOOPS_FOR_BARREL)
				{
					Barrel* barrel = new Barrel(barrelPosition);
					barrel->setMap(map);
					enemies.push_back(barrel); // Add barrel to enemies vector
					_gameLoop = 0;           // Reset game loop counter
				}

				if (mario.getLifeStatus()) // If Mario died
				{
					resetStage(enemies, &mario, &map, &hammer); // Reset stage

					if (results.popResult() != pair{ iteration, Results::gotHit }) 
					{
						reportResultError("Results file doesn't match mario got hit event!", _fileNames[stage], iteration);
						unmatching_result_found = true;
						break;
					}
					nextGotHitIteration = results.getNextGotHitIteration();
				}
				else
				{
					if (iteration == nextGotHitIteration && nextGotHitIteration != 0) 
					{
						reportResultError("Results file has a hit event that didn't happen in the game!", _fileNames[stage], iteration);
						unmatching_result_found = true;
						break;
					}
				}

				if (mario.getHammerStatus()) // If Mario has a hammer
				{
					map.drawHammerUses(map.getLPosition(), hammer.getUses()); // Draw hammer uses
				}

				// Handle hammer use exhaustion
				if (hammer.getUses() == 0)
				{
					map.eraseHammerUses(map.getLPosition()); // Erase hammer uses from screen
					hammer.setUses(-1); // Prevent further drawing of hammer uses
				}

				// Update score display if it has changed
				if (_scoreChange)
				{
					_scoreChange = false;
					map.drawScore(map.getLPosition(), _score); // Draw updated score
				}

				++iteration;
			} // end of game loop


			if(!unmatching_result_found)
			{
				// Game over condition
				if (mario.getLife() == 0)
				{
					if (results.popResult() != pair{ iteration, Results::gameOver })
					{
						reportResultError("Results file doesn't match Game Over event! terminating load", _fileNames[stage], iteration);
						unmatching_result_found = true;
					}
					else
						stage = _fileNames.size(); // End the loop if Mario loses all lives
				}
				else if (mario.rescuedPauline()) // Mario wins
				{

					if (results.popResult() != pair{ iteration, Results::rescudedPauline })
					{
						reportResultError("Results file doesn't match rescuded Pauline event!", _fileNames[stage], iteration);
						unmatching_result_found = true;
					}
					else
					{
						score += 5;
						_score += 5; // Bonus score for rescuing Pauline
					}
				}
				else if (results.popResult() == pair{ iteration, Results::noResult })
				{
					reportResultError("you must have been pressing N ha?\n YOU CHEATER!!!!!", _fileNames[stage], iteration);
					unmatching_result_found = true;
				}

				if (score != results.getScore()) // checks if curr map score match load file score
				{
					reportResultError("Results file doesn't match score value of the game!", _fileNames[stage], iteration);
					unmatching_result_found = true;
				}
			}

			// delete all objects from container 
			for (const auto& enemy : enemies)
			{
				delete enemy;
			}
		}
	}
	loadValidScreen();
}



void AutomaticGame::reportResultError(const string& message, const string& filename, size_t iteration)
{
	system("cls");
	cout << "Screen " << filename << " - " << message << '\n';
	cout << "Iteration: " << iteration << '\n';
	cout << "Press any key to continue to next screens (if any)" << endl;
	_getch();
}

void AutomaticGame::loadValidScreen()
{
	clrscr(); // Clear the screen for a fresh start
	// Load Valid Screen layout
	const char* validScreenMap[GameConfig::GAME_HEIGHT] = {
		"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL", // 0
		"L                                                                              L", // 1
		"L                                                                              L", // 2
		"L                         GGG   GGG    !!!  GGG    GGG                        L", // 3
		"L                        G      G      !!!  G      G                           L", // 4
		"L                        G   Gh G  GG  !!!  G  GG  G  GG                       L", // 5
		"L                        G   G  G   G  !!!  G   G  G   G                       L", // 6
		"L                         GGG    GGG   !!!   GGG    GGG                        L", // 7
		"L                                                                              L", // 8
		"L                   Your save file is safe and sound!                          L", // 9
		"L                                                                              L", // 10
		"L            The princess is waiting for you in this castle!                   L", // 11
		"L                                                                              L", // 12
		"L                Bowser is sharpening his shell hurry up!                      L", // 13
		"L                                                                              L", // 14
		"L                     Save Code: MARIO-GG-GOOD-200                             L", // 15
		"L                                                                              L", // 16
		"L            Press any key to start your epic Mario adventure!                 L", // 17
		"L                                                                              L", // 18
		"L                                                                              L", // 19
		"L                                                                              L", // 20
		"L                                                                              L", // 21
		"L                                                                              L", // 22
		"L                                                                              L", // 23
		"LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"  // 24
	};

	// Print the valid screen
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << validScreenMap[i] << endl;
	}
	cout << validScreenMap[GameConfig::GAME_HEIGHT - 1];

	// Wait for user input to proceed
	_ch = _getch();

}
