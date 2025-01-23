#pragma once

#include "point.h" // Handles positions and coordinates
#include "gameConfig.h" // Game configuration and constants
#include "map.h" // Map-related functionalities

class Hammer
{
private:
    Point _firstHitPos; // Position of the first hit
    Point _secondHitPos; // Position of the second hit
    Map* _pMap = nullptr; // Pointer to the map object
    int _uses = -1; // Number of uses, initialized to -1 until Mario collects the hammer

public:
    Hammer() : _uses(-1) {} // Initializes the hammer with -1 uses

    void hit(const Point& firstHit, const Point& secondHit); // Performs a hammer hit
    void lift(); // Lifts the hammer
    void reset(); // Resets the hammer to its initial state

    int getUses() const { return _uses; } // Gets the remaining uses of the hammer
    void setUses(const int use) { _uses = use; } // Sets the number of hammer uses
    void setMap(Map& map) { _pMap = &map; } // Sets a reference to the map
};
