#pragma once

#include <cstdlib> // For rand()
#include "gameConfig.h" // Game configuration and constants
#include <vector> // For vector container
#include "point.h" // Handles positions and coordinates
#include "map.h" // Map-related functionalities
#include "enemy.h" // Base class for enemies

using namespace std;

class Map; // Forward declaration
class Point; // Forward declaration

class SmartGhost: public Enemy
{

private:
    bool _isDestroyed = false; // Indicates if SmartGhost is destroyed

    // Helper methods for ghost movement !
    void changeDirection(); // Change ghost direction
    void changeDirectionRandomly(); // Randomly change direction

public:
    // Constructor
    SmartGhost(const Point& startPos) : Enemy(startPos, { 1, 0 }) {}

    // Override virtual functions from Enemy class !
    virtual void draw() const override { Enemy::draw((char)GameConfig::utilKeys::SMART_GHOST); }
    virtual void move() override; // Implement ghost movement logic
    virtual void reset() override; // Reset ghost to initial state

    // Self functions
    void setStatus(bool status) { _isDestroyed = status; }
    bool isDestroyed() const { return _isDestroyed; }

};

