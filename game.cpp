#pragma once
#include "game.h"

using namespace std;

void Game::menu()
{
    ShowConsoleCursor(false); // Hide the console cursor

    getAllMapsFileNames(); // get all files name into the vector 
    sort(_fileNames.begin(), _fileNames.end());  // Sort the vector in ascending order
}

void Game::resetStage(vector<Enemy*>& enemies, Mario* pMario, Map* pMap, Hammer* pHammer)
{
    // Resets all enemies, erasing the barrels from the map and deleting them from the vector, and reseting the ghosts to their starting positions
    for (size_t i = 0; i < enemies.size();)
    {
        Enemy* enemy = enemies[i];

        // Check if the enemy is a barrel, if it's a barrel, cast the pointer to Barrel*, otherwise return nullptr
        Barrel* barrel = dynamic_cast<Barrel*>(enemy);

        if (barrel) // if enemy is a barrel
        {
            if(barrel->isDestroyed()) // if the barrel is exploded
                barrel->clearExplosion(); // Clear explosion from the map

            barrel->reset();
            delete barrel; // deleting Barrel* barrel
            enemies.erase(enemies.begin() + i); // deleting the spot in the vector that was a pointer to barrel
        }
        else // it's a ghost / smart ghost // other enemy
        {
            enemy->reset();
            ++i;
        }
    }


    // Reset the hammer object to its initial state
    pHammer->reset();

    // Reset Mario's state when he dies
    pMario->reset();

    // Reset game loop and active barrels counters
    _gameLoop = 0;

    // Prepare for updating the life status on the map
    Point erasePos(pMap->getLPosition());
    erasePos.setX(erasePos.getX() + 7); // Set position to erase the life count (Life:...)

    // Erase the old life count and draw the new life count after reset
    pMap->eraseLife(erasePos, pMario->getLife() + 1); // Erase life on the map
    pMap->drawLife(pMap->getLPosition(), pMario->getLife()); // Draw updated life

    // Erase hammer uses display if applicable
    pMap->eraseHammerUses(pMap->getLPosition());
}

void Game::getAllMapsFileNames()
{
    // Alias the std::filesystem namespace as fs for convenience
    namespace fs = std::filesystem;

    // Iterate through all entries in the current directory
    for (const auto& entry : fs::directory_iterator(fs::current_path()))
    {
        // Get the filename of the current entry
        auto filename = entry.path().filename();

        // Convert the filename to a string
        auto filenameStr = filename.string();

        // Check if the filename starts with "dkong" and has a ".screen" extension
        if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen")
        {
            // If conditions are met, add the filename to the vector
            _fileNames.push_back(filenameStr);
        }
    }
}

void Game::noMapsErrorScreen()
{
    clrscr(); // Clear the screen when resuming
    // No Maps Error screen layout
    const char* noMapsScreenMap[GameConfig::GAME_HEIGHT] = {
        "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", // 0
        "E                                                                              E", // 1
        "E                                                                              E", // 2
        "E                         OOO   OOO   PPPP   SSSS   !!!                        E", // 3
        "E                        O   O O   O  P   P S       !!!                        E", // 4
        "E                        O   O O   O  PPPP   SSSS   !!!                        E", // 5
        "E                        O   O O   O  P         S   !!!                        E", // 6
        "E                         OOO   OOO   P     SSSS    !!!                        E", // 7
        "E                                                                              E", // 8
        "E                 Uh-oh! It looks like no maps are available!                  E", // 9
        "E                                                                              E", // 10
        "E              The library is empty... Maybe Bowser hid them all!              E", // 11
        "E                     Or the Princess has misplaced them...                    E", // 12
        "E                                                                              E", // 13
        "E                       Error Code: MISSING-MAP-404                            E", // 14
        "E                                                                              E", // 15
        "E                        Press any key to return...                            E", // 16
        "E                                                                              E", // 17
        "E                                                                              E", // 18
        "E                                                                              E", // 19
        "E                                                                              E", // 20
        "E                                                                              E", // 21
        "E                                                                              E", // 22
        "E                                                                              E", // 23
        "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"  // 24
    };

    // Print the No Maps Error Screen
    for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; ++i)
    {
        cout << noMapsScreenMap[i] << endl;
    }
    cout << noMapsScreenMap[GameConfig::GAME_HEIGHT - 1];

    // Wait for user input to return to the menu
    _ch = _getch();
}
