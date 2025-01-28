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

protected:
    char _ch = '\0'; // Stores user input
    size_t _levelNum = 0; // Current level number
    int _gameLoop = 0; // Counter for game loops
    vector<string> _fileNames; // Stores all map file names
    int _score = 0; // Current score
    bool _scoreChange = false; // Tracks if the score has changed
    
    // shared methods for all game modes
    void getAllMapsFileNames(); // Retrieves all map file names from the folder
    void resetStage(vector<Enemy*>& enemies, Mario* pMario, Map* pMap, Hammer* pHammer); // Resets the stage (resets Mario, enemies, map, and hammer)
    void noMapsErrorScreen(); // Displays an error screen when no maps are found

public:
    Game() :  _ch('\0'), _gameLoop(0), _score(0), _scoreChange(false) {} // Default constructor
	Game(const Game&) = delete; // Deletes the copy constructor
	Game& operator=(const Game&) = delete; // Deletes the copy assignment operator
	virtual ~Game() = default;

	// Virtual Functions
	virtual void run() = 0; // Runs the game
	virtual void menu(); // Displays the menu

};