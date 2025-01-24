#pragma once

#include "game.h"
#include "manualGame.h"
#include "automaticGame.h"



int main(int argc, char** argv)
{
    vector<string> args(argv, argv + argc);
	Game* game = nullptr;

    bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
    bool isSilent = isLoad && argc > 2 && string(argv[2]) == "-silent";
    bool isSave = argc > 1 && string(argv[1]) == "-save";

   if (isLoad)
   {
	   game = new AutomaticGame(isSilent);
       game->run(); // Start loading last game save
   }
   else
   {
	   game = new ManualGame(isSave);
	   game->menu(); // Start the game
   }    
}


/*  TO DO LIST
_________________________
// poli game ( virtual run )
// in normal run we need to add save.
// ghost need to be a base class ( smart ghost and dumb ghost)
// we need to save seed of ghost
// 2 more classes - one is steps and second is results
// while in isSilent do we need to print the map only if(!isSilent)
// 
*/

