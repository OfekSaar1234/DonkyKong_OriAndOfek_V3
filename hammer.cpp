#include "hammer.h"

// This function is called when the hammer hits, it updates the map with '~' at the hit positions
// and decreases the number of uses of the hammer by 1
void Hammer::hit(const Point& firstHit, const Point& secondHit)
{
	_uses -= 1; // Decrease the hammer's uses by 1
	_firstHitPos = firstHit; // Store the first hit position
	_secondHitPos = secondHit; // Store the second hit position

	// Check if either of the hit positions contains a Barrel or a Ghost
	if (_pMap->getCharCurrentMap(firstHit) == (char)GameConfig::utilKeys::BARREL ||
		_pMap->getCharCurrentMap(firstHit) == (char)GameConfig::utilKeys::GHOST ||
		_pMap->getCharCurrentMap(secondHit) == (char)GameConfig::utilKeys::BARREL ||
		_pMap->getCharCurrentMap(secondHit) == (char)GameConfig::utilKeys::GHOST)
	{
		// Ensure the original position is not an EDGE before updating the current map
		if (_pMap->getCharOriginalMap(_firstHitPos) != (char)GameConfig::utilKeys::EDGE)
		{
			_pMap->updateCurrMap(firstHit, '~'); // Update the current map to show '~' at the hit position
			if (_pMap->getCharOriginalMap(_secondHitPos) != (char)GameConfig::utilKeys::EDGE)
			{
				_pMap->updateCurrMap(secondHit, '~'); // Similarly update second hit position
			}
		}
	}

	// Visual feedback on the console to show the hit (using '~' symbol)
	if (_pMap->getCharOriginalMap(_firstHitPos) != (char)GameConfig::utilKeys::EDGE) // do not draw ~ when near EDGE
	{
		if (!_pMap->getIsSilent())
		{
			gotoxy(firstHit.getX(), firstHit.getY()); // Move cursor to first hit position
			cout << '~'; // Display the '~' symbol at the first hit position
		}
		if (_pMap->getCharOriginalMap(_secondHitPos) != (char)GameConfig::utilKeys::EDGE) // do not draw ~ when near EDGE
		{
			gotoxy(secondHit.getX(), secondHit.getY()); // Move cursor to second hit position
			cout << '~'; // Display the '~' symbol at the second hit position
		}
	}
}

// This function "lifts" the hammer, restoring the original characters at the hit positions
void Hammer::lift()
{
	char originalFirstHit = _pMap->getCharOriginalMap(_firstHitPos); // Store the original character at the first hit position
	char originalSecondHit = _pMap->getCharOriginalMap(_secondHitPos); // Store the original character at the second hit position

	// Check if the original character at the first hit position is not EDGE and restore it
	if (_pMap->getCharOriginalMap(_firstHitPos) != (char)GameConfig::utilKeys::EDGE)
	{
		if (!_pMap->getIsSilent())
		{
			gotoxy(_firstHitPos.getX(), _firstHitPos.getY()); // Move cursor to the first hit position
			cout << originalFirstHit; // Restore the original character at the first hit position
			if (_pMap->getCharOriginalMap(_secondHitPos) != (char)GameConfig::utilKeys::EDGE)
			{
				gotoxy(_secondHitPos.getX(), _secondHitPos.getY()); // Move cursor to the second hit position
				cout << originalSecondHit; // Restore the original character at the second hit position
			}
		}
	}

	// Update the current map with the original characters for both hit positions
	if (_pMap->getCharOriginalMap(_firstHitPos) != (char)GameConfig::utilKeys::EDGE)
	{
		_pMap->updateCurrMap(_firstHitPos, originalFirstHit); // Update current map at first hit position
		if (_pMap->getCharOriginalMap(_secondHitPos) != (char)GameConfig::utilKeys::EDGE)
		{
			_pMap->updateCurrMap(_secondHitPos, originalSecondHit); // Update current map at second hit position
		}
	}
}

// This function resets the hammer, restores its uses, and updates the map to place the hammer back in its initial position
void Hammer::reset()
{
	_uses = GameConfig::NUM_OF_P_USES; // Reset hammer uses 
	_pMap->updateOriginMap(_pMap->getHammerPosition(), (char)GameConfig::utilKeys::HAMMER); // Place the hammer back in its original position
	if (!_pMap->getIsSilent())
	{
		gotoxy(_pMap->getHammerPosition().getX(), _pMap->getHammerPosition().getY()); // Move cursor to hammer's position
		cout << (char)GameConfig::utilKeys::HAMMER; // Print the hammer at the original position
	}
}
