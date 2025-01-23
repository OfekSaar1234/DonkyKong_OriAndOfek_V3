#pragma once
#include "point.h" 
#include "general.h"
#include "gameConfig.h" 
#include "map.h" 
#include "enemy.h" // Base class for enemies

class Map; // Forward declaration

class Barrel : public Enemy
{
private:
    int _count_falling = 0; // Counter for falling steps

public:

    Barrel(const Point& startPos) : Enemy(startPos, { 0, 1 }) {}

    // Override virtual functions from Enemy class !
    virtual void draw() const override { Enemy::draw((char)GameConfig::utilKeys::BARREL); }
    virtual void move() override; // Implement barrel movement logic
    virtual void reset() override; // Reset barrel to initial state

    // Additional methods specific to Barrel ! 
    void explosion(); // Trigger barrel explosion
    void clearExplosion(); // Clear explosion effects
};
