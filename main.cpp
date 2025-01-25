#pragma once

#include "game.h"
#include "manualGame.h"
#include "automaticGame.h"



int main(int argc, char** argv)
{
	Game* game = nullptr;

    bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
    bool isSilent = isLoad && argc > 2 && string(argv[2]) == "-silent";
    bool isSave = argc > 1 && string(argv[1]) == "-save";

   if (isLoad)
   {
	   game = new AutomaticGame(isSilent);
       game->menu(); // Start loading last game save
   }
   else
   {
	   game = new ManualGame(isSave);
	   game->menu(); // Start the game
   } 

   delete game;
}


/*  TO DO LIST
_________________________
// while in isSilent do we need to print the map only if(!isSilent)
// delete to all 
// change map/screen functions and add the ones for Automatic game for errors in load
// readme file that contains format for steps and result files
*/

