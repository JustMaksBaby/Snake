#pragma once
#include "Segment.h"
#include <list> 
#include <Windows.h>

extern  const int g_ScreenWidth; 
extern const int g_ScreenHeight; 

class Food
{
public: 
	Segment  position;

private:
	wchar_t  symbol = L'$'; 
//COLOR INFO
	int whiteColor = 15; // base color for the char 


public: 
	Food(); 
	Food(short x, short y, wchar_t symbol); 
	
	void ChangeFoodPosition(HANDLE hConsole, const std::list<Segment>& snakeBody); 
	void DrawFood(wchar_t * screenBuffer); 
	void SetCharColor(HANDLE console,  int newColor); 


};