#pragma once

#include <Windows.h>
#include <process.h> // For system
#include <iostream>

void gotoxy(int x, int y); // Go to x,y position
void clrscr(); // Clear screen
void ShowConsoleCursor(bool showFlag); // Show/hide console cursor 
void setConsoleColor(int color);



