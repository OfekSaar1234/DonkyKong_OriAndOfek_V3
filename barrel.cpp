#pragma once
#include "barrel.h"
using namespace std;

void Barrel::move() // Move Barrel
{
    int currX = getPos().getX();
    int currY = getPos().getY();

    Point newPosition; // Barrel's new position
    Point charBelow = { currX, currY + 1 }; // Point below Barrel

    // Check if Barrel is on the floor
    if (!_pMap->isFloor(charBelow)) // If there's no floor below Barrel
    {
        _dir.x = 0;
        _dir.y = 1; // Free fall if there's no floor below Barrel
        _count_falling++; // Increment falling counter
    }

    newPosition = Point(currX + _dir.x, currY + _dir.y); // Calculate new position

    if (_pMap->isFloor(newPosition)) // Upon collision with floor
    {
        if (_count_falling >= GameConfig::NUM_OF_CHARS_FOR_BARREL_EXPLODE) // If Barrel fell enough to explode
        {
            this->explosion(); // Explode Barrel
        }
        else
            _count_falling = 0; // Reset falling counter

        if (_pMap->isLfloor(newPosition)) // If Barrel is on the left floor
            _dir = { -1, 0 }; // Move left
        else if (_pMap->isRfloor(newPosition)) // If Barrel is on the right floor
            _dir = { 1, 0 }; // Move right
        else
            _dir = { 0, 0 }; // Stop moving if neither left nor right floor
    }
    else // Update position if no collision occurs
    {
        setPosition(newPosition); // Update Barrel's position
        _pMap->updateCurrMap(newPosition, (char)GameConfig::utilKeys::BARREL); // Update map with Barrel
    }
}

void Barrel::explosion()
{
    _isDestroyed = true;

    Point newPosition; // Barrel's new position

    int barrelX = getPos().getX();
    int barrelY = getPos().getY();
    int newX;
    int newY;

    // Create explosion pattern around the Barrel
    for (int i = 0; i < 5; i++) // Columns
    {
        for (int j = 2; j >= 0; j--) // Rows
        {
            newX = barrelX - 2 + i; // Calculate new x position starting from -2 from Barrel's x position
            newY = barrelY - j; // Calculate new y position starting from above Barrel's position
            newPosition = { newX, newY }; // Set new position

            _pMap->updateCurrMap(newPosition, (char)GameConfig::utilKeys::EXPLOSION); // Update map with explosion
            if (!_pMap->getIsSilent())
            {
                gotoxy(newX, newY); // Move cursor to new position
                cout << (char)GameConfig::utilKeys::EXPLOSION; // Display explosion on screen
            }
            
        }
    }
}

void Barrel::clearExplosion()
{
    Sleep(GameConfig::EXPLOSION_DELAY); // Delay to show the explosion for the user

    Point newPosition;

    int barrelX = getPos().getX(); // Barrel's x position
    int barrelY = getPos().getY(); // Barrel's y position
    int newX;
    int newY;
    char originalChar;

    // Clear explosion pattern from the map
    for (int i = 0; i < 5; i++) // Columns
    {
        for (int j = 2; j >= 0; j--) // Rows
        {
            newX = barrelX - 2 + i; // Calculate new x position
            newY = barrelY - j; // Calculate new y position
            newPosition = { newX, newY }; // Set new position
            originalChar = _pMap->getCharOriginalMap(newPosition); // Get original character at position

            _pMap->updateCurrMap(newPosition, originalChar); // Restore original character in map
            if (!_pMap->getIsSilent())
            {
                gotoxy(newX, newY); // Move cursor to new position
                cout << originalChar; // Display original character on screen
            }
        }
    }

    this->reset(); // Reset Barrel to the starting position
}

void Barrel::reset()
{
    this->erase(); // Erase Barrel from the map
}

