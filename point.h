#pragma once

class Point
{
    int _x, _y; // X and Y coordinates

public:

    Point() : _x(-1), _y(-1) {} // Default constructor initializes coordinates to 0
    Point(int x, int y) : _x(x), _y(y) {} // Constructor with parameters to set x and y

    // Getters
    int getX() const { return _x; } // Return the X coordinate
    int getY() const { return _y; } // Return the Y coordinate

    // Setters
    void setX(int x) { _x = x; } // Set the X coordinate
    void setY(int y) { _y = y; } // Set the Y coordinate

    bool operator==(const Point& other) const
    {
        return _x == other._x && _y == other._y; // Check if two points are equal
    }
};
