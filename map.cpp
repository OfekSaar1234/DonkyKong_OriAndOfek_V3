#pragma once
#include "map.h"

using namespace std;

bool Map::load(const string& filename)
{
	// Open the file
	ifstream screen_file(filename); // Open the file for reading
	if (!screen_file.is_open())  // If file cannot be opened, return false
	{
		return false;
	}
	else
	{
		int curr_row = 0; // Current row in the map
		int curr_col = 0; // Current column in the map
		char c; // Variable to store each character read from the file

		// Read the file character by character
		while (!screen_file.get(c).eof() && curr_row < GameConfig::GAME_HEIGHT)
		{
			if (c == '\n')  // Check if the character is a newline (end of a row)
			{
				// Handle end of line: Ensure all rows are the correct width
				if (curr_col < GameConfig::GAME_WIDTH)
				{
					// If there are missing columns, add spaces
					#pragma warning(suppress : 4996) // Suppress warning for strcpy
					strcpy(_originalMap[curr_row] + curr_col, string(GameConfig::GAME_WIDTH - curr_col, ' ').c_str());
				}
				++curr_row;  // Move to the next row
				curr_col = 0;  // Reset column counter
				continue;
			}

			// Process each character if within map width
			if (curr_col < GameConfig::GAME_WIDTH)
			{
				// Handle special characters representing game entities

				// Check if the character is Mario
				if (c == (char)GameConfig::utilKeys::MARIO)
				{
					_marioPosition = { curr_col, curr_row };  // Store Mario's position
					_originalMap[curr_row][curr_col++] = ' '; // Replace Mario's position with a space
				}
				// Check if the character is Pauline
				else if (c == (char)GameConfig::utilKeys::PAULINE)
				{
					if (_paulinePosition == Point{ -1, -1 }) // first time '$' appears
					{
						_paulinePosition = { curr_col, curr_row };  // Store Pauline's position
						_originalMap[curr_row][curr_col++] = c; // Place Pauline in the map
					}
					else
					{
						_originalMap[curr_row][curr_col++] = ' '; // Place Pauline in the map
					}
				}
				// Check if the character is a Ghost (case insensitive)
				else if (c == (char)GameConfig::utilKeys::GHOST)
				{
					_ghostPositions.push_back({ curr_col, curr_row }); // Store the position of Ghost
					_originalMap[curr_row][curr_col++] = ' '; // Replace Ghost's position with a space
				}
				else if (c == (char)GameConfig::utilKeys::SMART_GHOST)
				{
					_smartGhostPositions.push_back({ curr_col, curr_row }); // Store the position of Ghost
					_originalMap[curr_row][curr_col++] = ' '; // Replace Ghost's position with a space
				}
				// Check if the character is Donkey Kong
				else if (c == (char)GameConfig::utilKeys::DKONG)
				{
					if (_dkongPosition == Point{ -1, -1 }) // first time '&' appears
					{
						_dkongPosition = { curr_col, curr_row };  // Store Donkey Kong's position
						_originalMap[curr_row][curr_col++] = c; // Place Donkey Kong in the map
					}
					else
					{
						_originalMap[curr_row][curr_col++] = ' ';
					}
				}
				// Check if the character is a Hammer
				else if (tolower(c) == (char)GameConfig::utilKeys::HAMMER)
				{
					if (_hammerPosition == Point{ -1, -1 }) // first time 'p' appears
					{
						_hammerPosition = { curr_col, curr_row }; // Store Hammer's position
						_originalMap[curr_row][curr_col++] = c; // Place Hammer in the map
					}
					else
					{
						_originalMap[curr_row][curr_col++] = ' ';
					}
				}
				// Check if the character is a Life
				else if (c == (char)GameConfig::utilKeys::LIFE)
				{
					_lifePosition = { curr_col, curr_row }; // Store Life's position
					_originalMap[curr_row][curr_col++] = ' '; // Replace Life's position with a space
				}
				else if (c != '=' && c != 'H' && c != '<' && c != '>' && c != ' ' && c != 'X') // if there's an unrecognized char in the map
				{
					_originalMap[curr_row][curr_col++] = 'Q'; // For all other characters, just add them to the map
				}
				else
				{
					_originalMap[curr_row][curr_col++] = c; // For all other characters, just add them to the map
				}
			}
		}

		// Define a "default" error position to check for missing elements
		Point errorP(-1, -1);

		// Check if any essential elements (Life, Donkey Kong, Pauline, or Mario) are missing
		if (_lifePosition == errorP || _dkongPosition == errorP || _paulinePosition == errorP || _marioPosition == errorP)
		{
			screen_file.close(); // close the file after finished reading from it 
			return false; // Return false if any key element is missing from the map
		}
		else
		{
			// Determine the last row of the map (ensuring it doesn't exceed the game's height)
			int last_row = (curr_row < GameConfig::GAME_HEIGHT ? curr_row : GameConfig::GAME_HEIGHT);

			// Fill the rows up to the game height with spaces
			for (int row = last_row + 1; row < GameConfig::GAME_HEIGHT; ++row)
			{
				#pragma warning(suppress : 4996) // Suppress warning for strcpy
				strcpy(_originalMap[row], string(GameConfig::GAME_WIDTH, ' ').c_str());
			}

			// Place the floor row below the game height (not visible to the user)
			#pragma warning(suppress : 4996) // Suppress warning for strcpy
			strcpy(_originalMap[GameConfig::GAME_HEIGHT], string(GameConfig::GAME_WIDTH, '=').c_str());
		}

		screen_file.close(); // close the file after finished reading from it 
		return true;  // Return true if map is successfully loaded and all essential elements are present
	}
}


// Resets the current map to its original state
void Map::reset()
{
	for (int i = 0; i < GameConfig::GAME_HEIGHT; i++)
	{
		memcpy(_currentMap[i], _originalMap[i], GameConfig::GAME_WIDTH + 1);
	}
}

void Map::print() const
{
	if (!_isSilent)
	{
		for (int i = 0; i < GameConfig::GAME_HEIGHT - 1; i++)
		{
			for (int j = 0; j < GameConfig::GAME_WIDTH; j++)
			{
				char element = _currentMap[i][j];

				if (_useColors)
				{
					// Set colors based on element type
					switch (element)
					{
					case (char)GameConfig::utilKeys::FLOOR:
						setConsoleColor((char)GameConfig::colors::RED); // Red
						break;

					case (char)GameConfig::utilKeys::LFLOOR:
					case (char)GameConfig::utilKeys::RFLOOR:
						setConsoleColor((char)GameConfig::colors::LIGHTBLUE); //light blue
						break;

					case (char)GameConfig::utilKeys::EDGE:
						setConsoleColor((char)GameConfig::colors::BLUE); // Blue
						break;

					case (char)GameConfig::utilKeys::PAULINE:
						setConsoleColor((char)GameConfig::colors::GOLD); // gold
						break;

					default:
						setConsoleColor((char)GameConfig::colors::WHITE); // Default console color
						break;
					}
				}
				// Print the character
				cout << element;
			}
			cout << '\n';
		}

		cout << _currentMap[GameConfig::GAME_HEIGHT - 1]; // Print the last line

		if (_useColors)
		{
			setConsoleColor((char)GameConfig::colors::WHITE); // Reset to default color for printing mario and barrel
		}
	}
}


char Map::getCharCurrentMap(const Point& point) const     // Get character at specified coordinates
{
	return _currentMap[point.getY()][point.getX()]; // Get character at specified coordinates
}

char Map::getCharOriginalMap(const Point& point) const
{
	return _originalMap[point.getY()][point.getX()]; // Get character at specified coordinates
}

bool Map::isEdge(const Point& point) const
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::EDGE; // Check if point is edge
}


bool Map::isFloor(const Point& point) const
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::FLOOR ||
		getCharOriginalMap(point) == (char)GameConfig::utilKeys::LFLOOR ||
		getCharOriginalMap(point) == (char)GameConfig::utilKeys::RFLOOR; // Check if point is floor in any kind
}

bool Map::isOutOfBound(const Point& point) const
{
	return point.getX() == GameConfig::GAME_WIDTH || point.getX() == -1 || point.getY() == GameConfig::GAME_HEIGHT || point.getY() == -1;
}

bool Map::isLadder(const Point& point) const // Check if point is ladder
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::LADDER;
}


bool Map::isRfloor(const Point& point) const // Check if point is right floor
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::RFLOOR;
}

bool Map::isLfloor(const Point& point) const // Check if point is left floor
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::LFLOOR;
}

bool Map::isSpace(const Point& point) const // Check if point is space
{
	return getCharOriginalMap(point) == (char)GameConfig::utilKeys::SPACE;
}


void Map::updateCurrMap(const Point& point, const char ch) // Update current map state
{
	_currentMap[point.getY()][point.getX()] = ch;
}

void Map::updateOriginMap(const Point& point, const char ch)
{
	_originalMap[point.getY()][point.getX()] = ch;
}

void Map::drawLife(const Point& point, const int life) const // Draw life
{
	if (!_isSilent)
	{
		gotoxy(point.getX(), point.getY()); // go coord of life and draw '@ ' loop of number of current lifes
		cout << "Lives: ";
		for (int i = 0; i < life; ++i)
		{
			cout << "@ ";
		}
	}
	
}

void Map::eraseLife(const Point& point, const int prevLife) const // Erase
{
	if (!_isSilent)
	{
		gotoxy(point.getX(), point.getY());
		for (int i = 0; i < prevLife; ++i) // erase the previous life (draw '  ' loop of number of previous lifes)
		{
			cout << "  ";
		}
	}
}

void Map::drawScore(const Point& point, const int score) const
{
	if (!_isSilent)
	{
		gotoxy(point.getX(), point.getY() + 1); // go coord of score and draw _score number of current score
		cout << "Score: " << score;
	}
}

void Map::drawHammerUses(const Point& point, const int uses) const
{
	if (!_isSilent)
	{
		gotoxy(point.getX() + 13, point.getY() + 2); // deletes the prev number of uses 
		cout << "  ";
		gotoxy(point.getX(), point.getY() + 2);
		cout << "P uses left: " << uses;
	}
}

void Map::eraseHammerUses(const Point& point) const
{
	if (!_isSilent)
	{
		gotoxy(point.getX(), point.getY() + 2); // deletes the prev number of uses 
		cout << "               ";
	}
}
