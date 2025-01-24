//#include "automaticGame.h"
//
//void AutomaticGame::run()
//{
//	// Ask user for color choice before starting the game
//	char colorChoice;
//	cout << "Do you want to play with colors? (enter - y/Y for yes/and any other key for no):" << endl;
//	cin >> colorChoice; // Get user input
//	clrscr(); // Clear screen
//	bool useColors = (colorChoice == 'y' || colorChoice == 'Y'); // true for 'y' or 'Y', false otherwise
//
//	bool isPaused = false; // Flag for game pause state
//
//	int gameLoop = 0; // Game loop counter
//
//	Mario mario; // Create Mario character
//
//	if (_fileNames.empty()) // If no maps are available
//	{
//		noMapsErrorScreen(); // Display error message
//	}
//
//	// Loop through each stage in the game
//	for (size_t stage = _levelNum; stage < _fileNames.size(); stage++)
//	{
//		clrscr(); // Clear screen for the new stage
//		Map map; // Create a new map object
//		//long random_seed;
//		//Steps steps;
//		//Results results;
//
//		if (map.load(_fileNames[stage])) // If map successfully loads
//		{
//			map.reset();   // Reset map to initial state
//			map.enableColors(useColors); // Enable or disable colors based on user input
//			map.print();   // Print the map
//
//			mario.resetToNextStage(map.getMarioPosition()); // Reset Mario's position for the new stage
//
//			map.drawLife(map.getLPosition(), mario.getLife()); // Draw Mario's life count on the map
//			map.drawScore(map.getLPosition(), _score); // Draw score on the map
//
//			vector<Enemy*> enemies; // containert for all the enemies in the stage
//
//			Hammer hammer; // Create a Hammer object
//			hammer.setMap(map); // Set map for the hammer
//
//			mario.setMap(map);   // Link Mario to the current map
//			mario.setHammer(hammer); // Link hammer to Mario
//
//			Point barrelPosition = map.getDkongPosition(); // Get Donkey Kong's position
//
//			// Set the barrel starting position depending on Donkey Kong's location
//			if (map.getCharOriginalMap({ barrelPosition.getX() - 1, barrelPosition.getY() }) == (char)GameConfig::utilKeys::EDGE)
//			{
//				barrelPosition.setX(barrelPosition.getX() + 1); // Barrel starts from the right
//			}
//			else
//			{
//				barrelPosition.setX(barrelPosition.getX() - 1); // Barrel starts from the left
//			}
//
//			// Initialize ghosts
//			vector<Point> ghostPos = map.getGhostPositions();
//
//			// Initialize smart ghosts
//			vector<Point> smartGhostsPos = map.getSmartGhostPositions();
//
//			for (const auto& pos : ghostPos)
//			{
//				Ghost* ghost = new Ghost(pos);
//				ghost->setMap(map);
//				enemies.push_back(ghost);
//			}
//
//			for (const auto& pos : smartGhostsPos)
//			{
//				SmartGhost* smartGhost = new SmartGhost(pos);
//				smartGhost->setMap(map);
//				enemies.push_back(smartGhost);
//			}
//
//			isPaused = false; // Ensure game is not paused at the start
//
//			gameLoop = 0; // Reset game loop counter
//
//			// Game loop runs while Mario has life and hasn't rescued Pauline
//			while (mario.getLife() > 0 && !mario.rescuedPauline())
//			{
//				gameLoop = get_gameLoop(); // Get current game loop count
//
//				if (_kbhit()) // Check if a key is pressed
//				{
//					char keyPressed = _getch(); // Get pressed key
//
//					// Cheat code for skipping to the next stage
//					if (tolower(keyPressed) == 'n')
//					{
//						set_gameLoop(0);
//						break;
//					}
//
//					// Pause or resume game on pressing ESC
//					if (keyPressed == (char)GameConfig::utilKeys::ESC)
//					{
//						if (!isPaused) // First ESC pressed: pause the game
//						{
//							isPaused = true;
//							clrscr();
//							cout << "Game Paused. Press ESC to resume..." << endl;
//						}
//						else // Second ESC pressed: resume the game
//						{
//							isPaused = false;
//							clrscr(); // Clear screen to resume game
//							map.print(); // Redraw the map
//							map.drawLife(map.getLPosition(), mario.getLife());
//							map.drawScore(map.getLPosition(), _score);
//							if (mario.getHammerStatus()) // If Mario has a hammer
//							{
//								map.drawHammerUses(map.getLPosition(), hammer.getUses());
//							}
//							mario.draw(); // Redraw Mario
//
//							// Draw active enemies
//							for (const auto& enemy : enemies)
//							{
//								if (!enemy->isDestroyed()) // if this enemy isn't destroyed
//								{
//									enemy->draw();
//								}
//							}
//						}
//						continue;
//					}
//
//					if (!isPaused) // If the game is running
//					{
//						mario.keyPressed(keyPressed); // Handle Mario's key press
//					}
//				}
//
//				if (!isPaused) // Game is running, proceed with game logic
//				{
//					// Draw active barrels and ghosts
//					for (const auto& enemy : enemies)
//					{
//						if (!enemy->isDestroyed()) // if this enemy isn't destroyed
//						{
//							enemy->draw();
//						}
//					}
//
//					mario.draw(); // Draw Mario
//
//					Sleep(GameConfig::MOVE_DELAY); // Delay for smooth movement
//
//					// handles enemy's actions
//					for (size_t i = 0; i < enemies.size();)
//					{
//						Enemy* enemy = enemies[i];
//
//						// Check if the enemy is a barrel, if it's a barrel, cast the pointer to Barrel*, else return nullptr
//						Barrel* barrel = dynamic_cast<Barrel*>(enemy);
//
//						if (barrel && barrel->HitEdge()) // if barrel hits the edge
//						{
//							barrel->erase();// Erase barrel from screen
//							delete barrel;// Free memory
//							enemies.erase(enemies.begin() + i); // Remove from vector
//						}
//						else if (enemy->gotHit()) // if enemy got hit by mario's hammer
//						{
//							enemy->erase(); // erases enemy from screen
//
//							if (barrel) // if enemy is a Barrel
//							{
//								delete enemy;// Free memory
//								enemies.erase(enemies.begin() + i); // Remove from vector
//							}
//							else // enemy is a Ghost / SmartGhost
//							{
//								enemy->setStatus(true);
//							}
//
//							_score += 1;// Increase score
//							_scoreChange = true; // Mark score for redrawing
//							++i; // Continue to the next enemy
//						}
//						else if (barrel && barrel->isDestroyed()) // if it's a barrel if it got exploded
//						{
//							barrel->clearExplosion();
//							delete barrel;// Free memory
//							enemies.erase(enemies.begin() + i); // Remove from vector
//						}
//						else
//						{
//							enemy->erase();// Erase for movement effect
//							++i;// Continue to the next enemy
//						}
//					}
//
//					mario.erase(); // Erase Mario from the map
//
//					// Move enemy
//					for (const auto& enemy : enemies)
//					{
//						if (!enemy->isDestroyed()) // if this enemy is a ghost
//						{
//							enemy->move();
//						}
//					}
//
//					mario.move(); // Move Mario
//
//					set_gameLoop(gameLoop + 1); // Increment game loop counter
//
//					if (gameLoop >= GameConfig::LOOPS_FOR_BARREL)
//					{
//						Barrel* barrel = new Barrel(barrelPosition);
//						barrel->setMap(map);
//						enemies.push_back(barrel); // Add barrel to enemies vector
//						set_gameLoop(0);           // Reset game loop counter
//					}
//
//					if (mario.getLifeStatus()) // If Mario died
//					{
//						resetStage(enemies, &mario, &map, &hammer); // Reset stage
//					}
//
//					if (mario.getHammerStatus()) // If Mario has a hammer
//					{
//						map.drawHammerUses(map.getLPosition(), hammer.getUses()); // Draw hammer uses
//					}
//
//					// Handle hammer use exhaustion
//					if (hammer.getUses() == 0)
//					{
//						map.eraseHammerUses(map.getLPosition()); // Erase hammer uses from screen
//						hammer.setUses(-1); // Prevent further drawing of hammer uses
//					}
//
//					// Update score display if it has changed
//					if (_scoreChange)
//					{
//						_scoreChange = false;
//						map.drawScore(map.getLPosition(), _score); // Draw updated score
//					}
//				}
//				else // If the game is paused
//				{
//					Sleep(GameConfig::MOVE_DELAY); // Delay during pause
//				}
//			}
//
//			// Game over condition
//			if (mario.getLife() == 0)
//			{
//				stage = _fileNames.size(); // End the loop if Mario loses all lives
//				gameOverScreen(); // Show game over screen
//			}
//			else if (mario.rescuedPauline()) // Mario wins
//			{
//				_score += 5; // Bonus score for rescuing Pauline
//				if (stage == _fileNames.size() - 1) // We in the last stage/screen
//				{
//					gameWinningScreen(); // Show winning screen if last level
//				}
//				else
//				{
//					nextLevelScreen(); // Show next level screen
//				}
//			}
//
//			// delete all objects from container 
//			for (const auto& enemy : enemies)
//			{
//				delete enemy;
//			}
//		}
//		else // If the map fails to load
//		{
//			gameErrorScreen(); // Show error screen
//		}
//	}
//
//	// Reset game settings for new game
//	_levelNum = 0;
//	_score = 0;
//}