#pragma once

#include <iostream>

class GameConfig
{
public:
    // Game dimensions
    static constexpr int GAME_WIDTH = 80;// x
    static constexpr int GAME_HEIGHT = 25; // y

    // Movement delay
    static constexpr int MOVE_DELAY = 78;
    static constexpr int MOVE_DELAY_LOAD = 20;


    
    // Utility keys enum
    enum class utilKeys { SPACE = ' ', LADDER = 'H', FLOOR = '=', LFLOOR = '<', RFLOOR = '>',
                          MARIO = '@', BARREL = 'O', GHOST = 'x', SMART_GHOST = 'X', EDGE = 'Q',
                          EXPLOSION = '*', PAULINE = '$', HAMMER = 'p', DKONG = '&', LIFE = 'L', 
                          SMASH = '~', ESC = 27};

    // Movement keys enum
    enum class movementKeys { LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 'x', STAY = 's', HIT = 'p' };

    // colors keys enum
    enum class colors { RED = 12, LIGHTBLUE = 3, BLUE = 1, GOLD = 14, WHITE = 7 };

    // Direction struct for movement
    struct Direction { int x, y; };

    // Starting parameters for Mario
    static constexpr int NUM_OF_CHARS_FOR_MARIO_DIE = 5;
    static constexpr int START_LIFE_AMOUNT = 3;


    // Barrels parameters
    static constexpr int NUM_OF_CHARS_FOR_BARREL_EXPLODE = 8;
    static constexpr int EXPLOSION_DELAY = 25;
    static constexpr int LOOPS_FOR_BARREL = 23;

    //Hammer parameters
    static constexpr int NUM_OF_P_USES = 50;


};