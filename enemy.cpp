#include "enemy.h"

void Enemy::draw(char ch) const // Draws the enemy at its current position
{
    if (!_pMap->getIsSilent())
    {
        int ghostX = _position.getX();
        int ghostY = _position.getY();
        gotoxy(ghostX, ghostY); // Move cursor to the enemy's position
        cout << ch; // Display the character representing the enemy (e.g., ghost)
    }
}

void Enemy::erase() const // Erase the enemy from the map by restoring original character
{
    char originalChar = _pMap->getCharOriginalMap(_position); // Get the original character at the enemy's position
    if (!_pMap->getIsSilent())
    {
        gotoxy(_position.getX(), _position.getY()); // Move the cursor to the enemy's position
        cout << originalChar; // Print the original character to restore the map
    }
    _pMap->updateCurrMap(_position, originalChar); // Update the current map with the original character
}

bool Enemy::gotHit() const // Check if the enemy got hit by a hammer
{
    char currentChar = _pMap->getCharCurrentMap(_position); // Get the character at the enemy's current position

    Point nextPos = { _position.getX() + _dir.x, _position.getY() }; // Calculate the enemy's next position
    char nextChar = _pMap->getCharCurrentMap(nextPos); // Get the character at the enemy's next position

    // If the enemy's current or next position is the 'SMASH' character (e.g., hammer)
    if (currentChar == (char)GameConfig::utilKeys::SMASH || nextChar == (char)GameConfig::utilKeys::SMASH)
    {
        return true; // The enemy got hit by the hammer
    }
    return false; // No hit
}

bool Enemy::HitEdge() const
{
    Point newPosition = { getPos().getX() + _dir.x, getPos().getY() + _dir.y };
    return _pMap->getCharOriginalMap(newPosition) == (char)GameConfig::utilKeys::EDGE;
}