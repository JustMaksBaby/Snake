#include "Snake.h"



void Snake::GetKeyInput()
{
	//check if user pressed arrow keys 
	rightKey = GetAsyncKeyState(VK_RIGHT) & 0x8000;
	leftKey = GetAsyncKeyState(VK_LEFT) & 0x8000;
	upKey = GetAsyncKeyState(VK_UP) & 0x8000; 
	downKey = GetAsyncKeyState(VK_DOWN) & 0x8000;  

	if (vertical)
	{
		if (rightKey)
		{
			direction = 1;  
			vertical = false;
			horizontal = true;
		}

		if (leftKey)
		{
			direction = 3; 
			vertical = false;
			horizontal = true;
		} 
	} 
	else if(horizontal)
	{
		if (upKey)
		{
			direction = 0;  

			horizontal = false;
			vertical = true;
		}

		if (downKey)
		{
			direction = 2;  

			horizontal = false;
			vertical = true;
		}
	}

}

void Snake::Move()
{
	//move snake
	switch (direction)
	{
	case 0: // up 
	{
		body.push_front({ body.front().x, body.front().y - 1 });
	}break;
	case 1: // right
	{
		body.push_front({ body.front().x + 1 , body.front().y });
	}break;
	case 2: //down
	{
		body.push_front({ body.front().x, body.front().y + 1 });
	}break;
	case 3:// left
	{
		body.push_front({ body.front().x - 1, body.front().y });
	}break;
	}

	body.pop_back(); 
}

void Snake::AddSegment()
{
	body.push_back({ body.back().x - 1, body.back().y });
}
void Snake::CollisionDetection(int width, int height, int foodX, int foodY)
{
	if (BordersCollision(width, height) || snakeSegmentCollision())
	{
		this->isDead = true;
	}
	
	if (FoodCollision(foodX, foodY))
	{
		score++; 
	}
		
}

bool Snake::FoodCollision(int foodX , int foodY)
{
	//returns true if snake  came across a food
	if (body.front().x == foodX && body.front().y == foodY)
	{
		return true;
	}
	
	return false;
}

bool Snake::snakeSegmentCollision()
{
	for (auto segment = ++body.begin(); segment != body.end(); ++segment)
	{
		if (body.front().x == segment->x && body.front().y == segment->y)
		{
			return true; 
		}
	}

	return false; 
}

bool Snake::BordersCollision(int width, int height)
{
	for (auto segment = body.begin(); segment != body.end(); ++segment)
	{
		if (segment->x == width || segment->y == height || segment->x < 0 || segment->y < 3)
		{
			return true;
		}
	}

	return false; 

}

void Snake::DrawSnake(wchar_t* screenBuffer, int screenWidth)
{
	for (Segment segment : body)
	{
		screenBuffer[screenWidth * segment.y + segment.x] = isDead ? '+': 'o'; 
	}
	
	//change character for snake`s head; 
	screenBuffer[screenWidth * body.front().y + body.front().x] = isDead ? 'X' : '@'; 
}

void Snake::DrawScore(wchar_t* screenBuffer, int whereToDraw)
{ 
	wsprintfW(screenBuffer + whereToDraw, L"%d", score); 
}

Snake::Snake()
{ 
	body = { {16,5}, {17,5}, {18,5} }; 
}
