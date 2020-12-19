#include <Windows.h>
#include <list> 
#include <thread>
#include <time.h>
#include <chrono>

#include "Snake.h"
#include "Food.h"


#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Kernel32.lib")
 

//in characters
extern  const int g_ScreenWidth  = 80; 
extern  const int g_ScreenHeight = 30;


int main()
{
	std::srand(time(NULL)); 

//CREATE CONSOLE BUFFER 
	
	//buffer to form  screen state
	constexpr int BUFFER_SIZE = g_ScreenHeight * g_ScreenWidth;
	wchar_t* screenBuffer = new wchar_t[BUFFER_SIZE] {};
	 
	//set console buffer 
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleScreenBufferSize(hConsole, {g_ScreenWidth, g_ScreenHeight});
	SetConsoleActiveScreenBuffer(hConsole); 

	//count chars written in console
	DWORD bytesWritten = 0; 
	
	bool startGame = true;
//GAME LOOP
	while (true)
	{
		//draw a top bar
		const int SCOREINFO_LENGTH = wcslen(L"SCORE:") + 4; 
		const int SCORE_POSITION = g_ScreenWidth + 60 + wcslen(L"SCORE:");

		for (int i = 0; i < g_ScreenWidth; ++i)
		{
			screenBuffer[i] = L'=';
			screenBuffer[2 * g_ScreenWidth + i] = L'=';
		}
		wsprintfW(screenBuffer + g_ScreenWidth + 30, L"S.N.A.K.E");
		wsprintfW(screenBuffer + g_ScreenWidth + 60, L"SCORE:%d", 0); 


		//change color for score field
		DWORD coloredChars = 0;
		FillConsoleOutputAttribute(hConsole, 11, SCOREINFO_LENGTH, { 60,  1 }, &coloredChars);
		

	//OBJECTS ARRENGEMENT 	
		Snake snake =  Snake(); 
		Food food = Food(); 
	
		while (startGame)
		{ 
			// fill the buffer with empty spaces
			for (int i = g_ScreenWidth * 3; i < g_ScreenHeight * g_ScreenWidth; ++i)
			{
				screenBuffer[i] = ' ';
			}
		
		//DRAW FOOD
			food.DrawFood(screenBuffer); 

			//change color for food
			food.SetCharColor(hConsole, 10); 

		//GET USER INPUT
			//catch input from user and slow down the snake
			auto currentTime = std::chrono::system_clock::now();
			while ((std::chrono::system_clock::now() - currentTime) < ((snake.direction % 2 <= 1) ? std::chrono::milliseconds(120) : std::chrono::milliseconds(200)));
			{
				snake.GetKeyInput(); 
			}

			
			snake.CollisionDetection(g_ScreenWidth, g_ScreenHeight, food.position.x, food.position.y);  
			if (snake.FoodCollision(food.position.x, food.position.y))
			{
				food.ChangeFoodPosition(hConsole, snake.GetBodyInfo()); 
				snake.AddSegment(); 
			}
			snake.Move(); 

			if (snake.isDead)
			{
				startGame = false;
				
				//clean bg color in where the food was, so it becomes prepared for the next round 
				food.SetCharColor(hConsole, 15); 
			}


			snake.DrawSnake(screenBuffer, g_ScreenWidth); 
			snake.DrawScore(screenBuffer, SCORE_POSITION); 

			//Display frame
			WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0, 0 }, &bytesWritten);
		}

		const wchar_t* enterString = L"PRESS ENTER TO START";
		int count = (g_ScreenWidth / 2 - wcslen(enterString)); 
		wsprintfW(&screenBuffer[30 * g_ScreenHeight + 10 ],enterString ); 
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight* g_ScreenWidth, { 0, 0 }, & bytesWritten);

		if (GetAsyncKeyState(VK_RETURN) && 0x8000) // Enter pressed
		{
			startGame = true; 
		}
	} 
	return 0; 
}

