#pragma once
#include "mario.h" // Mario class header
#include "barrel.h" // Barrel class header
#include "ghost.h" // Ghost class header
#include "smartGhost.h" // SmartGhost class header
#include "enemy.h" // Base class for enemies

#include <iostream> // For input and output operations
#include <filesystem> // For handling file paths and directories
#include <fstream> // For file handling
#include <windows.h> // For Windows-specific functionality
#include <string> // For string handling
#include <vector> // For vector container
#include <conio.h> // For kbhit and getch functions
#include <algorithm> // For sort


class Mario; // Forward declaration
class Barrel; // Forward declaration
class Ghost; // Forward declaration

class Game {
private:
    char _ch = '\0'; // Stores user input
    size_t _levelNum = 0; // Current level number
    int _gameLoop = 0; // Counter for game loops
    vector<string> _fileNames; // Stores all map file names

    void run(); // Runs the game
    void MenuScreen(); // Displays the main menu
    void InstructionsScreen(); // Displays the instructions screen
    void levelsScreen(); // Displays levels for the user to choose
    void gameOverScreen(); // Displays the game over screen
    void gameWinningScreen(); // Displays the game-winning screen
    void nextLevelScreen(); // Displays the next level screen
    void getAllMapsFileNames(); // Retrieves all map file names from the folder
    void gameErrorScreen(); // Displays the error screen
    void noMapsErrorScreen(); // Displays an error screen when no maps are found
    int _score = 0; // Current score
    bool _scoreChange = false; // Tracks if the score has changed

public:
    Game() : _ch('\0'), _gameLoop(0), _score(0), _scoreChange(false) {} // Default constructor
    Game(const Game&) = delete; // Deletes the copy constructor
    Game& operator=(const Game&) = delete; // Deletes the copy assignment operator

    void menu(); // Displays the menu
    void resetStage(vector<Enemy*>& enemies, Mario* pMario, Map* pMap, Hammer* pHammer); // Resets the stage (resets Mario, enemies, map, and hammer)
    int get_gameLoop() const { return _gameLoop; } // Returns the game loop counter
    void set_gameLoop(int x) { _gameLoop = x; } // Sets the game loop counter
};
