#include <Windows.h>
#include <list> 
#include <thread>
#include <chrono>
#include <iostream>
#include <chrono>

#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Kernel32.lib")
 

//in characters
static const int g_ScreenWidth  = 80; 
static const int g_ScreenHeight = 30;  


struct SnakeSegment
{
	//position in chars 
	short x; 
	short y; 
};



int main()
{
	//create console buffer
	constexpr int BUFFER_SIZE = g_ScreenHeight * g_ScreenWidth;
	wchar_t* screenBuffer = new wchar_t[BUFFER_SIZE] {};

	//set console buffer 
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleScreenBufferSize(hConsole, {g_ScreenWidth, g_ScreenHeight});
	SetConsoleActiveScreenBuffer(hConsole); 

	//count chars written in console
	DWORD bytesWritten = 0; 

	//snake status
	bool isDead = false; 


	//pressed keys
	bool rightKey    = false; 
	bool leftKey     = false; 
	bool rightKeyOld = false;
	bool leftKeyOld  = false;

	

	while (true)
	{
		//draw a top bar
		const int SCOREINFO_LENGTH = wcslen(L"SCORE:") + 4;
		for (int i = 0; i < g_ScreenWidth; ++i)
		{
			screenBuffer[i] = L'=';
			screenBuffer[2 * g_ScreenWidth + i] = L'=';
		}

		wsprintfW(screenBuffer + g_ScreenWidth + 30, L"S.N.A.K.E");
		wsprintfW(screenBuffer + g_ScreenWidth + 60, L"SCORE:%d", 0);
		//change color for score area
		DWORD coloredChars = 0;
		FillConsoleOutputAttribute(hConsole, 11, SCOREINFO_LENGTH, { 60,  1 }, &coloredChars);
		int scoreValuePosition = 60 + wcslen(L"SCORE:");

		//count foods that we have eaten
		int  score = 0;


		//snake body
		std::list<SnakeSegment> snakeBody = { {60,15}, {61,15}, {62,15} }; 
		//snake direction
		int snakeDirection = 1; /*  snake direction 0 - up | 1 - right | 2 - down | 3 - left    */
		

		//food position
		// on map can be only one food element 
		short foodX = 15;
		short foodY = 13;
				
		while (!isDead)
		{ 
			// feel the buffer with empty spaces
			for (int i = g_ScreenWidth * 3; i < g_ScreenHeight * g_ScreenWidth; ++i)
			{
				screenBuffer[i] = ' ';
			}

			//draw a food 
			screenBuffer[g_ScreenWidth * foodY + foodX] = L'$';
			//change color for food
			DWORD  coloredChars = 0;
			int foodColor = 10;
			int whiteColor = 15;
			FillConsoleOutputAttribute(hConsole, foodColor, 1, { foodX,foodY }, &coloredChars);

			//catch input from user and slow down the snake
			auto currentTime = std::chrono::system_clock::now();
			while ((std::chrono::system_clock::now() - currentTime) < ((snakeDirection % 2 <= 1) ? std::chrono::milliseconds(120) : std::chrono::milliseconds(200)));
			{
				//check if user pressed arrow keys
				rightKey = GetAsyncKeyState(VK_RIGHT) & 0x8000;
				leftKey = GetAsyncKeyState(VK_LEFT) & 0x8000;

				if (rightKey && !rightKeyOld)
				{
					snakeDirection++;
					if (snakeDirection == 4)
					{
						snakeDirection = 0;
					}
					rightKeyOld = true;
				}
				rightKeyOld = rightKey;


				if (leftKey && !leftKeyOld)
				{
					snakeDirection--;
					if (snakeDirection == -1)
					{
						snakeDirection = 3;
					}
					leftKeyOld = true;
				}
				leftKeyOld = leftKey;

			}

			//move snake
			switch (snakeDirection)
			{
			case 0: // up 
			{
				snakeBody.push_front({ snakeBody.front().x, snakeBody.front().y + 1 });
			}break;
			case 1: // right
			{
				snakeBody.push_front({ snakeBody.front().x - 1 , snakeBody.front().y });
			}break;
			case 2: //down
			{
				snakeBody.push_front({ snakeBody.front().x, snakeBody.front().y - 1 });
			}break;
			case 3:// left
			{
				snakeBody.push_front({ snakeBody.front().x + 1, snakeBody.front().y });
			}break;
			}


			//collision detection for boundaries 
			if (snakeBody.front().x < 0 || snakeBody.front().x >= g_ScreenWidth)
			{
				isDead = true;
			}
			if (snakeBody.front().y < 3 || snakeBody.front().y >= g_ScreenHeight)
			{
				isDead = true;
			}

			//collision detection with food
			if (snakeBody.front().x == foodX && snakeBody.front().y == foodY)
			{
				score++;
				//update score value on the screen
				wsprintfW(screenBuffer + g_ScreenWidth + scoreValuePosition, L"%d", score); 

				//change place color where the food was to standart
				FillConsoleOutputAttribute(hConsole, whiteColor, 1, { foodX,foodY }, &coloredChars);

				//find a new place for the food 
				while (screenBuffer[foodY * g_ScreenWidth + foodX] != L' ')
				{
					foodX = rand() % g_ScreenWidth;
					foodY = rand() % (g_ScreenHeight - 3) + 3;
				}
			}
			else
			{
				snakeBody.pop_back();
			}

			//collision detenction when snake touches itself
			for (std::list<SnakeSegment>::iterator i = snakeBody.begin(); i != snakeBody.end(); ++i)
			{

				if (i != snakeBody.begin() && i->x == snakeBody.front().x && i->y == snakeBody.front().y)
				{
					isDead = true;
				}
			}


			//draw snake body
			for (auto segment : snakeBody)
			{
				screenBuffer[g_ScreenWidth * segment.y + segment.x] = isDead ? L'+' : L'0';
			}
			//draw  snake`s head 
			screenBuffer[g_ScreenWidth * snakeBody.front().y + snakeBody.front().x] = isDead ? L'X' : L'@';



			//Display frame
			WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0, 0 }, &bytesWritten);
		}

		const wchar_t* enterString = L"PRESS ENTER TO START";
		int count = (g_ScreenWidth / 2 - wcslen(enterString)); 
		wsprintfW(&screenBuffer[30 * g_ScreenHeight + 10 ],enterString ); 
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight* g_ScreenWidth, { 0, 0 }, & bytesWritten);

		if (GetAsyncKeyState(VK_RETURN) && 0x8000)
		{
			isDead = false; 
		}
	} 
	return 0; 
}