#pragma once
#include "point.h"
#include "gameConfig.h"
#include "map.h"

class Enemy
{
private:
    Point _startPos; // Starting position of the enemy
    Point _position; // Current position of the enemy

protected:
    GameConfig::Direction _dir; // Current direction of movement
    Map* _pMap = nullptr; // Pointer to the map object
    bool _isDestroyed = false; // Indicates if the enemy is destroyed
    void draw(char ch) const; // Protected method to draw the enemy

public:

    Enemy(const Point& startPos, const GameConfig::Direction dir) : _startPos(startPos), _position(startPos), _dir(dir) {}

    virtual ~Enemy() = default; // Virtual destructor !


    // Getter and setter methods for position
    Point getStartPos() const { return _startPos; }
    Point getPos() const { return _position; }
    void setStartPos(const Point& point) { _startPos = point; }
    void setPosition(const Point& point) { _position = point; }
    void setStatus(bool status) { _isDestroyed = status; }

    // Common methods for all enemies
    void erase() const; // Erase the enemy from the screen
    void setMap(Map& map) { _pMap = &map; } // Set the map reference
    bool gotHit() const; // Check if the enemy was hit
    bool HitEdge() const; // Check if enemy hit an edge


    // virtual functions !
    virtual void move() = 0; // Move the enemy
    virtual void draw() const = 0; // Draw the enemy
    virtual void reset() = 0; // Reset the enemy to its initial state
    virtual bool isDestroyed() const = 0; // Check if the enemy is destroyed

};
