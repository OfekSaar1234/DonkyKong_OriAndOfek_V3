#pragma once
#include "general.h" // General utilities
#include "point.h" // For handling positions and coordinates
#include "map.h" // Map-related functionalities
#include "hammer.h" // Hammer-related functionalities
#include <vector> // For vector container

using namespace std;

class Map; // Forward declaration
class Hammer; // Forward declaration

class Mario
{
private:
    Point _startPos; // Starting position
    Point _position; // Mario's current position
    Point _newPosition; // Potential new position for Mario
    GameConfig::Direction _dir{ 0, 0 }; // Direction Mario is moving (default: stationary)
    Map* _pMap = nullptr; // Pointer to the map object
    Hammer* _pHammer = nullptr; // Pointer to the hammer object

    int _jumpCounter = 0; // Counter for jump height
    int _countFalling = -1; // Counter for falling
    int _life = 3; // Mario's life count
    bool _isJumping = false; // Indicates if Mario is jumping
    bool _died = false; // Indicates if Mario has died
    bool _isFalling = false; // Indicates if Mario is falling
    bool _hasHammer = false; // Indicates if Mario has a hammer
    bool _usedHammer = false; // Indicates if Mario used the hammer

    void draw(char ch) const; // Draw character at Mario's position
    void useHammer(); // Handle hammer usage

public:
    Mario() : _jumpCounter(0), _countFalling(-1), _life(3), _isJumping(false), _died(false), _isFalling(false), _hasHammer(false), _usedHammer(false) {} // Default constructor
    Mario(const Mario&) = delete; // Delete copy constructor to prevent copying Mario
    Mario& operator=(const Mario&) = delete; // Delete copy assignment operator to prevent assignment

    void draw() { draw(_hasHammer ? 'M' : (char)GameConfig::utilKeys::MARIO); } // Draw Mario with hammer or default appearance

    void erase(); // Erase Mario from the screen

    // Get functions
    int getX() const { return _position.getX(); } // Get Mario's X position
    int getY() const { return _position.getY(); } // Get Mario's Y position
    int getLife() const { return _life; } // Get Mario's life count
    bool getLifeStatus() const { return _died; } // Check if Mario has died
    bool getHammerStatus() const { return _hasHammer; } // Check if Mario has a hammer

    // Set functions
    void setMap(Map& map) { _pMap = &map; } // Set reference to the map
    void setHammer(Hammer& hammer) { _pHammer = &hammer; } // Set reference to the hammer

    void keyPressed(char key); // Handle key press events
    void jump(); // Perform jump action
    void climb(char key); // Perform climbing action
    void move(); // Move Mario based on current direction
    bool gotHit() const; // Check if Mario got hit by a barrel or explosion
    bool rescuedPauline() const; // Check if Mario reached Pauline
    void reset(); // Reset Mario upon death
    void resetToNextStage(const Point& point); // Reset Mario for the next stage
};
