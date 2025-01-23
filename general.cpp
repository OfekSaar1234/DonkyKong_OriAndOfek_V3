#pragma once
#include "general.h"


using namespace std;


// Note: The functions here are used for basic console manipulation,
// such as moving the cursor and clearing the screen and adding colors.
// gotoxy, clrscr, ShowConsoleCursor and setConsoleColor are from Keren's code "snakes" 

// Moves the console cursor to the specified (x, y) position
void gotoxy(int x, int y)
{
    HANDLE hConsoleOutput;
    COORD dwCursorPosition;
    cout.flush();
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

// Clears the console screen
void clrscr()
{
    system("cls");
}

// Shows or hides the console cursor based on the showFlag parameter
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void setConsoleColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
