#pragma once

#include "ghost.h"

void Ghost::move()
{
    int currX = getPos().getX();
    int currY = getPos().getY();

    // Calculate new position for the ghost based on its current direction
    Point newPosition(currX + _dir.x, currY + _dir.y);
    Point newFurtherPosition(currX + (2 * _dir.x), currY); // Check two steps ahead to see if ghost will collide
    Point CharBelow = { currX , currY + 1 };
    Point nextCharBelow = { currX + _dir.x, currY + 1 }; // Check the point below the ghost to avoid movement over edges

    if (_pMap->isSpace(CharBelow)) // free falling 
    {
        _dir = { 0, 1 }; // falling
    }
    else if (_pMap->isFloor(CharBelow)) // about to hit a floor 
    {
        if (_dir.x == 0) // Ghost was falling
        {
            _dir = { 1, 0 }; // our def movement of ghost is right
        }

        if (!_pMap->isFloor(nextCharBelow) || _pMap->getCharCurrentMap(newPosition) == (char)GameConfig::utilKeys::GHOST ||
            _pMap->getCharCurrentMap(newFurtherPosition) == (char)GameConfig::utilKeys::GHOST ||
            _pMap->isEdge(newPosition) || _pMap->isFloor(newPosition)) // If ghost encounters an obstacle (no floor or another ghost), change direction
        {
            changeDirection(); // Change direction if necessary (hit wall or another ghost)
        }
        else
        {
            changeDirectionRandomly(); // Otherwise, change direction randomly with a small chance
        }
    }

    // Update the ghost's position after determining the new direction
    newPosition = Point(currX + _dir.x, currY + _dir.y); // Recalculate position after direction change

    setPosition(newPosition); // Set the new position for the ghost
    _pMap->updateCurrMap(newPosition, (char)GameConfig::utilKeys::GHOST); // Update map with the ghost's new position
}

void Ghost::changeDirection()
{
    // Reverse direction: if moving right, change to left, and vice versa
    if (_dir.x == 1)
        _dir.x = -1; // Change direction to left
    else
        _dir.x = 1; // Change direction to right
}

void Ghost::changeDirectionRandomly()
{
    // Generate a random number between 0 and 99 for a chance to change direction
    int randomChance = rand() % 100;

    // 5% chance to change direction (randomly alter movement)
    if (randomChance < 5)
    {
        changeDirection(); // Change direction if the random chance is met
    }
}

void Ghost::reset()
{
    this->erase(); // Erase the ghost from the screen
    _isDestroyed = false; // Set the ghost's destroyed flag to false
    setPosition(getStartPos()); // Reset ghost's position to its starting point
}
