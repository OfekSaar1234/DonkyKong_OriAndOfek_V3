#pragma once

#include "point.h" // Handles positions and coordinates
#include <iostream>
#include "gameConfig.h" // Game configuration and constants
#include "general.h" // General utilities
#include <filesystem> // For handling file paths
#include <fstream> // For file operations
#include <windows.h> // For Windows-specific functionality
#include <string> 
#include <vector> // For vector container
#include <conio.h> // For kbhit and getch functions

using namespace std;

class Point; // Forward declaration

class Map
{
private:
    bool _useColors = false; // Indicates if colors are enabled (default: false)
    bool _isSilent = false;
    char _originalMap[GameConfig::GAME_HEIGHT + 1][GameConfig::GAME_WIDTH + 1]; // Original map state (+1 for null terminator)

    char _currentMap[GameConfig::GAME_HEIGHT + 1][GameConfig::GAME_WIDTH + 1]; // Current map state (+1 for null terminator)

    Point _lifePosition; // Position of life counter
    Point _dkongPosition; // Position of Donkey Kong
    Point _paulinePosition; // Position of Pauline
    Point _marioPosition; // Position of Mario
    Point _hammerPosition; // Position of the hammer
    vector<Point> _ghostPositions; // Positions of ghosts
    vector<Point> _smartGhostPositions; // Position of Smart Ghosts

public:
    Map() : _lifePosition({ -1, -1 }), _dkongPosition({ -1, -1 }), _paulinePosition({ -1, -1 }), _marioPosition({ -1, -1 }), _isSilent(false), _useColors(false) {} // Constructor initializes positions to invalid!!! state
    Map(const Map&) = delete; // Delete copy constructor to prevent copying
    Map& operator=(const Map&) = delete; // Delete copy assignment operator to prevent assignment

    bool load(const std::string& filename); // Load map from file

    void enableColors(bool useColors) { _useColors = useColors; } // Enable or disable colors
    void reset(); // Reset map to original state
    void print() const; // Print the current map state
    char getCharCurrentMap(const Point& point) const; // Get character from the current map state
    char getCharOriginalMap(const Point& point) const; // Get character from the original map state
    bool isEdge(const Point& point) const; // Check if a point is an edge
    bool isFloor(const Point& point) const; // Check if a point is a floor
    bool isOutOfBound(const Point& point) const; // check if a point is out of screen bounds
    bool isLadder(const Point& point) const; // Check if a point is a ladder
    bool isRfloor(const Point& point) const; // Check if a point is a right floor
    bool isLfloor(const Point& point) const; // Check if a point is a left floor
    bool isSpace(const Point& point) const; // Check if a point is empty space
    void updateCurrMap(const Point& point, const char ch); // Update the current map state
    void updateOriginMap(const Point& point, const char ch); // Update the original map state

    void drawLife(const Point& point, const int life) const; // Draw the life counter
    void eraseLife(const Point& point, const int prevLife) const; // Erase the life counter
    void drawScore(const Point& point, const int score) const; // Draw the score
    void drawHammerUses(const Point& point, const int uses) const; // Draw hammer uses
    void eraseHammerUses(const Point& point) const; // Erase hammer uses

    bool getUseColors() const { return _useColors; } // Check if colors are enabled

    Point getLPosition() const { return _lifePosition; } // Get life position
    Point getDkongPosition() const { return _dkongPosition; } // Get Donkey Kong position
    Point getPaulinePosition() const { return _paulinePosition; } // Get Pauline position
    Point getMarioPosition() const { return _marioPosition; } // Get Mario position
    Point getHammerPosition() const { return _hammerPosition; } // Get hammer position
    vector<Point> getGhostPositions() const { return _ghostPositions; } // Get ghost positions
    vector<Point> getSmartGhostPositions() const { return _smartGhostPositions; } // Get smart ghost positions

    void setIsSilent(bool isSilentMode) {_isSilent = isSilentMode;}
};
