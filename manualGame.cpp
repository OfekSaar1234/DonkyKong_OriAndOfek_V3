#include "manualGame.h"
#include "steps.h"
#include "results.h"

// Displays the main menu and handles user input for menu options.
void ManualGame::menu()
{
	ShowConsoleCursor(false); // Hide the console cursor

	getAllMapsFileNames(); // get all files name into the vector 
	sort(_fileNames.begin(), _fileNames.end());  // Sort the vector in ascending order

	// Options 
	while (true)
	{
		MenuScreen(); // Display the menu screen
		char choice = _getch();

		switch (choice)
		{
		case '1':
			clrscr();
			this->run(); // Start a new game session from first map
			break;

		case '2':
			levelsScreen();
			break;

		case '8':
			InstructionsScreen(); // Display game instructions
			break;

		case '9':
			system("cls"); // Clear the screen
			return; // Exit menu loop and end program

		default:
			break; // Ignore invalid inputs
		}
	}
}

void ManualGame::run()
{
	// Ask user for color choice before starting the game
	char colorChoice;
	cout << "Do you want to play with colors? (enter - y/Y for yes/and any other key for no):" << endl;
	cin >> colorChoice; // Get user input
	clrscr(); // Clear screen
	bool useColors = (colorChoice == 'y' || colorChoice == 'Y'); // true for 'y' or 'Y', false otherwise

	bool isPaused = false; // Flag for game pause state

	Mario mario; // Create Mario character

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

		Steps steps;
		Results results;

		size_t iteration = 0; // we need iteration to be outside the loop
		size_t score = 0;

		string filename_prefix = _fileNames[stage].substr(0, _fileNames[stage].find_last_of('.')); // dKong_01 for example
		string stepsFilename = filename_prefix + ".steps";  // dKong_01.steps
		string resultsFilename = filename_prefix + ".result"; // dKong_01.result
		
		if (map.load(_fileNames[stage])) // If map successfully loads
		{
			map.reset();   // Reset map to initial state
			map.enableColors(useColors); // Enable or disable colors based on user input
			map.print();   // Print the map

			random_seed = static_cast<long>(chrono::system_clock::now().time_since_epoch().count());

			steps.setColorMode(useColors);
			steps.setRandomSeed(random_seed);

			srand(random_seed);

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

			isPaused = false; // Ensure game is not paused at the start

			// Game loop runs while Mario has life and hasn't rescued Pauline
			while (mario.getLife() > 0 && !mario.rescuedPauline())
			{
				if (_kbhit()) // Check if a key is pressed
				{
					char keyPressed = _getch(); // Get pressed key

					// Cheat code for skipping to the next stage
					if (tolower(keyPressed) == 'n')
					{
						_gameLoop = 0;
						break;
					}

					// Pause or resume game on pressing ESC
					if (keyPressed == (char)GameConfig::utilKeys::ESC)
					{
						if (!isPaused) // First ESC pressed: pause the game
						{
							isPaused = true;
							clrscr();
							cout << "Game Paused. Press ESC to resume..." << endl;
						}
						else // Second ESC pressed: resume the game
						{
							isPaused = false;
							clrscr(); // Clear screen to resume game
							map.print(); // Redraw the map
							map.drawLife(map.getLPosition(), mario.getLife());
							map.drawScore(map.getLPosition(), _score);
							if (mario.getHammerStatus()) // If Mario has a hammer
							{
								map.drawHammerUses(map.getLPosition(), hammer.getUses());
							}
							mario.draw(); // Redraw Mario

							// Draw active enemies
							for (const auto& enemy : enemies)
							{
								if (!enemy->isDestroyed()) // if this enemy isn't destroyed
								{
									enemy->draw();
								}
							}
						}
						continue;
					}

					if (!isPaused && keyPressed != (char)GameConfig::utilKeys::ESC) // If the game is running
					{
						mario.keyPressed(keyPressed); // Handle Mario's key press

						if(_isSave && validKey(keyPressed))
						{
							if(tolower(keyPressed) == (char)GameConfig::utilKeys::HAMMER)
							{
								if(mario.getHammerStatus())
									steps.addStep(iteration, keyPressed);
							}
							else
								steps.addStep(iteration, keyPressed);
						}
					}
				}

				if (!isPaused) // Game is running, proceed with game logic
				{
					// Draw active barrels and ghosts
					for (const auto& enemy : enemies)
					{
						if (!enemy->isDestroyed()) // if this enemy isn't destroyed
						{
							enemy->draw();
						}
					}

					mario.draw(); // Draw Mario

					Sleep(GameConfig::MOVE_DELAY); // Delay for smooth movement

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
						if (_isSave)
						{
							results.addResult(iteration, Results::gotHit);
						}

						resetStage(enemies, &mario, &map, &hammer); // Reset stage
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
				}
				else // If the game is paused
				{
					Sleep(GameConfig::MOVE_DELAY); // Delay during pause
				}
				++iteration;
			}

			// Game over condition
			if (mario.getLife() == 0)
			{
				if (_isSave)
					results.addResult(iteration, Results::gameOver);

				stage = _fileNames.size(); // End the loop if Mario loses all lives
				gameOverScreen(); // Show game over screen
			}
			else if (mario.rescuedPauline()) // Mario wins
			{
				if (_isSave)
					results.addResult(iteration, Results::rescudedPauline);

				score += 5;
				_score += 5; // Bonus score for rescuing Pauline
				if (stage == _fileNames.size() - 1) // We in the last stage/screen
				{
					gameWinningScreen(); // Show winning screen if last level
				}
				else
				{
					nextLevelScreen(); // Show next level screen
				}
			}

			results.addScore(score);

			// delete all objects from container 
			for (const auto& enemy : enemies)
			{
				delete enemy;
			}
			if (_isSave)
			{
				steps.saveSteps(stepsFilename); // create the new steps file
				results.saveResults(resultsFilename); // create the new results file
			}
		}
		else // If the map fails to load
		{
			gameErrorScreen(); // Show error screen
		}
	}

	// Reset game settings for new game
	_levelNum = 0;
	_score = 0;
}


void ManualGame::levelsScreen()
{
	clrscr(); // Clear the screen to show the level selection screen

	cout << "\n === Available Levels ===\n\n";
	int counter = 1; // Initialize a counter for the levels (1-based index)

	// Loop through the file names representing levels
	for (const auto& fileName : _fileNames)
	{
		cout << counter << ". " << fileName << endl; // Display the level number and its name
		++counter; // Increment the counter for the next level
	}

	cout << "\n\nEnter the level number to select: ";

	// Wait for user input to select a level
	size_t level;
	cin >> level; // Read the selected level number

	level -= 1;  // Convert the input to 0-based index (since user sees 1-based numbering)

	// Check if the level number is valid
	if (0 <= level && level < _fileNames.size() && _fileNames.size() >= 1)
	{
		_levelNum = level; // Set the selected level number
	}
}



void ManualGame::gameOverScreen()
{
	clrscr(); // Clear the screen when resuming
	// Game Over screen layout
	const char* gameOverMap[GameConfig::GAME_HEIGHT] = {
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		"Q                                                                              Q", // 1
		"Q                                                                              Q", // 2
		"Q                                                                              Q", // 3
		"Q                              G A M E   O V E R                               Q", // 4
		"Q                                                                              Q", // 5
		"Q                                                                              Q", // 6
		"Q                           Better luck next time!                             Q", // 7
		"Q                                                                              Q", // 8
		"Q                                                                              Q", // 9
		"Q                     Press any key to return to the menu...                   Q", // 10
		"Q                                                                              Q", // 11
		"Q                                                                              Q", // 12
		"Q                                                                              Q", // 13
		"Q                                                                              Q", // 14
		"Q                                                                              Q", // 15
		"Q                                                                              Q", // 16
		"Q                                                                              Q", // 17
		"Q                                                                              Q", // 18
		"Q                                                                              Q", // 19
		"Q                                                                              Q", // 20
		"Q                                                                              Q", // 21
		"Q                                                                              Q", // 22
		"Q                                                                              Q", // 23
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};

	// Print the Game Over screen
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << gameOverMap[i] << endl;
	}
	cout << gameOverMap[GameConfig::GAME_HEIGHT - 1];
	// Wait for user input to return to the menu
	_ch = _getch();

	//check mario life restore ////////////////////
}

void ManualGame::gameWinningScreen()
{
	clrscr(); // Clear the screen when resuming
	// Game Winning screen layout
	const char* winningScreenMap[GameConfig::GAME_HEIGHT] = {
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		"Q                                                                              Q", // 1
		"Q                                                                              Q", // 2
		"Q                         W   W   III   N   N   !!!                            Q", // 3
		"Q                         W   W    I    NN  N   !!!                            Q", // 4
		"Q                         W W W    I    N N N   !!!                            Q", // 5
		"Q                          W W    III   N  NN   !!!                            Q", // 6
		"Q                                                                              Q", // 7
		"Q                                                                              Q", // 8
		"Q                                                                              Q", // 9
		"Q                      C O N G R A T U L A T I O N S !                         Q", // 10
		"Q                                                                              Q", // 11
		"Q                  The princess was rescued by the brave Mario                 Q", // 12
		"Q                                                                              Q", // 13
		"Q                    Press any key to return to the menu...                    Q", // 14
		"Q                                                                              Q", // 15
		"Q                                                                              Q", // 16
		"Q                           *   *   *   *   *   *                              Q", // 17
		"Q                            * * * * * * * * * *                               Q", // 18
		"Q                             *   *   *   *   *                                Q", // 19
		"Q                                                                              Q", // 20
		"Q                                                                              Q", // 21
		"Q                                                                              Q", // 22
		"Q                                                                              Q", // 23
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};

	// Print the Winning Screen
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << winningScreenMap[i] << endl;
	}
	cout << winningScreenMap[GameConfig::GAME_HEIGHT - 1];

	// Wait for user input to return to the menu
	_ch = _getch();
}

void ManualGame::nextLevelScreen()
{
	clrscr(); // Clear the screen when resuming
	// Game Winning screen layout
	const char* winningScreenMap[GameConfig::GAME_HEIGHT] = {
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		"Q                                                                              Q", // 1
		"Q                                                                              Q", // 2
		"Q                                                                              Q", // 3
		"Q                   OH NO! DONKEY KONG KIDNAPPED THE PRINCESS!                 Q", // 4
		"Q                                                                              Q", // 5
		"Q                    Mario was too slow to reach her in time...                Q", // 6
		"Q                    Donkey Kong took her to the next stage!                   Q", // 7
		"Q                                                                              Q", // 8
		"Q                                                                              Q", // 9
		"Q            Mario, you must hurry! Go to the next stage and rescue her!       Q", // 10
		"Q                                                                              Q", // 11
		"Q                             *   *   *   *   *   *                            Q", // 12
		"Q                              * * * * * * * * * *                             Q", // 13
		"Q                               *   *   *   *   *                              Q", // 14
		"Q                                                                              Q", // 15
		"Q                                                                              Q", // 16
		"Q                     Press any key to move to the next level!!                Q", // 17
		"Q                                                                              Q", // 18
		"Q                                                                              Q", // 19
		"Q                                                                              Q", // 20
		"Q                                                                              Q", // 21
		"Q                                                                              Q", // 22
		"Q                                                                              Q", // 23
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};

	// Print the Winning Screen
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << winningScreenMap[i] << endl;
	}
	cout << winningScreenMap[GameConfig::GAME_HEIGHT - 1];

	// Wait for user input to return to the menu
	_ch = _getch();
}


void ManualGame::gameErrorScreen()
{
	clrscr(); // Clear the screen when resuming
	// Game Winning screen layout
	const char* errorScreenMap[GameConfig::GAME_HEIGHT] = {
		"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", // 0
		"E                                                                              E", // 1
		"E                                                                              E", // 2
		"E                         OOO   OOO   PPPP   SSSS   !!!                        E", // 3
		"E                        O   O O   O  P   P S       !!!                        E", // 4
		"E                        O   O O   O  PPPP   SSSS   !!!                        E", // 5
		"E                        O   O O   O  P         S   !!!                        E", // 6
		"E                         OOO   OOO   P     SSSS    !!!                        E", // 7
		"E                                                                              E", // 8
		"E                     Something went terribly wrong!                           E", // 9
		"E                                                                              E", // 10
		"E               It seems the princess is in another castle...                  E", // 11
		"E                     Or maybe Bowser ate your map !                           E", // 12
		"E                                                                              E", // 13
		"E                     Error Code: MAMA-MTA-404                                 E", // 14
		"E                                                                              E", // 15
		"E                Press any key to return and debug this mess...                E", // 16
		"E                                                                              E", // 17
		"E                                                                              E", // 18
		"E                                                                              E", // 19
		"E                                                                              E", // 20
		"E                                                                              E", // 21
		"E                                                                              E", // 22
		"E                                                                              E", // 23
		"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"  // 24
	};

	// Print the error Screen
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << errorScreenMap[i] << endl;
	}
	cout << errorScreenMap[GameConfig::GAME_HEIGHT - 1];

	// Wait for user input to return to the menu
	_ch = _getch();
}




void ManualGame::MenuScreen()
{
	clrscr(); // Clear the screen when resuming
	// Menu layout screen
	const char* menuMap[GameConfig::GAME_HEIGHT] = {
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		"Q                                                                              Q", // 1
		"Q                                                                              Q", // 2
		"Q                      WELCOME TO DONKEY KONG CLASSIC                          Q", // 3
		"Q                                                                              Q", // 4
		"Q                      Developed by: Ofek Saar & Ori Ratzabi                   Q", // 5
		"Q                                                                              Q", // 6
		"Q                       1. Start a New Game                                    Q", // 7
		"Q                       2. levels Screens                                      Q", // 8
		"Q                       8. Instructions                                        Q", // 9
		"Q                       9. Exit                                                Q", // 10
		"Q                                                                              Q", // 11
		"Q                       Enter Your Choice:                                     Q", // 12
		"Q                                                                              Q", // 13
		"Q                       Prepare to Climb, Jump, and be amazed!                 Q", // 14
		"Q                                                                              Q", // 15
		"Q                                                                              Q", // 16
		"Q           Note: You can choose specific level option 2 (level screen)        Q", // 17
		"Q                 and start playing at the level you chose                     Q", // 18
		"Q                                                                              Q", // 19
		"Q               If no level has been picked (or no existing level),            Q", // 20
		"Q                        the game will start at level 1                        Q", // 21
		"Q                                                                              Q", // 22
		"Q                                                                              Q", // 23
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};

	// Print the menu  to the console
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << menuMap[i] << endl;
	}
	cout << menuMap[GameConfig::GAME_HEIGHT - 1];

}

void ManualGame::InstructionsScreen()
{
	clrscr(); // Clear the screen when resuming

	// instructions layout screen
	const char* instructionScreen[GameConfig::GAME_HEIGHT] = 
	{
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		"Q                                                                              Q", // 1
		"Q                                                                              Q", // 2
		"Q                               Instructions:                                  Q", // 3
		"Q                                                                              Q", // 4
		"Q                               a/A - Move Left                                Q", // 5
		"Q                               d/D - Move Right                               Q", // 6
		"Q                               w/W - Jump/Climb Up                            Q", // 7
		"Q                               x/X - Climb Down                               Q", // 8
		"Q                               s/S - Stay                                     Q", // 9
		"Q                               ESC - Pause Game                               Q", // 10
		"Q                                                                              Q", // 11
		"Q  !!!Be aware of barrels and their explosion you will die if you get close!!! Q", // 12
		"Q        !!!Be aware of ghosts, once you touch a ghost you will die as well    Q", // 13
		"Q                                                                              Q", // 14
		"Q     If you kill a ghost or smash the barrel you will get an extra point!!!   Q", // 15
		"Q     If you save the princes in each level you get an extra 5 point!          Q", // 16
		"Q                                                                              Q", // 17
		"Q                                                                              Q", // 18
		"Q                                                                              Q", // 19
		"Q                      Press any key to return to menu...                      Q", // 20
		"Q                                                                              Q", // 21
		"Q                                                                              Q", // 22
		"Q                                                                              Q", // 23
		"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};

	// Print the instruction layout to the console
	for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
	{
		cout << instructionScreen[i] << endl;
	}
	cout << instructionScreen[GameConfig::GAME_HEIGHT - 1];
	// Wait for user input to return to the menu
	_ch = _getch();

	//return cursur to start 
}


bool ManualGame::validKey(const char keyPressed) const
{
	string validKeys = "wsdaxpn"; // List of valid keys
	return validKeys.find(tolower(keyPressed)) != string::npos; // .find function return "string::npos" if the key isn't in the string validKeys
}
