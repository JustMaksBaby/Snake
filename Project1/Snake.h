#pragma once 
#include "Segment.h"
#include "Food.h"

#include <Windows.h>
#include <list>
class Snake
{
private:
	std::list<Segment> body;
	int score = 0; 
public: 
	//eaten food
	//pressed keys
	bool rightKey = false;
	bool leftKey = false;
	bool upKey = false;
	bool downKey = false;
	
	//snake position 
	bool horizontal = true;
	bool vertical = false;

	//snake direction
	int direction = 3;  /*  snake direction 0 - up | 1 - right | 2 - down | 3 - left    */

	//snake state
	bool isDead = false; 

public : 
	void GetKeyInput(); 

	void Move(); 
	void AddSegment(); 
	void CollisionDetection(int width, int height, int foodX, int foodY);  
	bool FoodCollision(int x, int y);  

	void DrawSnake(wchar_t* screenBuffer, int screenWidth);
	void DrawScore(wchar_t* screenBuffer, int whereToDraw); 

	const std::list<Segment>& GetBodyInfo()
	{
		return body; 
	}
	

	Snake();
private: 
	bool BordersCollision(int width, int height); 
	bool snakeSegmentCollision(); 

	

}; 