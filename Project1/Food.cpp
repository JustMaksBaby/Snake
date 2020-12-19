#include "Food.h"

Food::Food()
{
	position = Segment{15, 13 };
}

Food::Food(short x, short y, wchar_t symbol)
{
	position.x = x; 
	position.y = y; 
	this->symbol = symbol; 
}

void Food::ChangeFoodPosition(HANDLE hConsole, const std::list<Segment>& snakeBody)
{
	short newX = rand() % g_ScreenWidth; 
	short newY = (rand() % (g_ScreenHeight-3)) + 3; 

	bool satisfied = false; 
	while (!satisfied)
	{
		satisfied = true; 

		for (auto snakeSegment : snakeBody)
		{
			if (snakeSegment.x == newX || snakeSegment.y == newY)
			{
				satisfied = false; 

				newX = rand() % g_ScreenWidth;
				newY = (rand() % (g_ScreenHeight - 3) ) + 3; 

				break; 
			}
		}
	}
	
	//change bg color for the place where the food was eaten; 
	this->SetCharColor(hConsole, 15); // 15 means WHITE

	position.x = newX; 
	position.y = newY; 

 
}

void Food::DrawFood(wchar_t* screenBuffer)
{
	screenBuffer[g_ScreenWidth * position.y + position.x] = symbol; 
}

void Food::SetCharColor(HANDLE hConsole, int newColor)
{
	//value for console can be in range 0-15
	if (newColor > 15) 
	{
		newColor = 15; 
	}

	//change color for food
	DWORD  coloredChars = 0;
	FillConsoleOutputAttribute(hConsole, newColor, 1, { position.x, position.y }, &coloredChars);
}
