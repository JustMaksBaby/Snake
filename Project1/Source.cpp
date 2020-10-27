#include <Windows.h>
#include <list> 
#include <thread>
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


void  InitTopBar(wchar_t * buffer)
{
	for (int i = 0; i < g_ScreenWidth; ++i)
	{
		buffer[i] = L'='; 
		buffer[2 * g_ScreenWidth + i] = L'=';
	}

	wsprintfW(buffer + g_ScreenWidth + 30, L"S.N.A.K.E"); 
}

int main()
{
	//create console buffer
	constexpr int BUFFER_SIZE = g_ScreenHeight * g_ScreenWidth;
	wchar_t* screenBuffer = new wchar_t[BUFFER_SIZE] {};
	//init  map


	//set console buffer 
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleScreenBufferSize(hConsole, {g_ScreenWidth, g_ScreenHeight});
	SetConsoleActiveScreenBuffer(hConsole); 

	//count chars written in console
	DWORD bytesWritten = 0; 

	//GENERAL INFO
	bool isDead = false; 
	//count foods that we have eaten
	int  score = 0; 

	/*
		snake direction 
			0 - up
			1 - right
			2 - down
			3 - left
	*/
	int snakeDirection = 1;  
	//pressed keys
	bool rightKey    = false; 
	bool leftKey     = false; 
	bool rightKeyOld = false;
	bool leftKeyOld  = false;

	//snake body
	std::list<SnakeSegment> snakeBody = { {60,15}, {61,15}, {62,15} };  


	//food position
	// on map can be only one food element 
	short foodX = 15; 
	short foodY = 13; 

	
	InitTopBar(screenBuffer);  
	//game loop 
	while (1)
	{
		//slow down the game
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

		//draw snake body
		for (int i = g_ScreenWidth* 3; i < g_ScreenHeight * g_ScreenWidth; ++i)
		{
			screenBuffer[i] = ' '; 
		}
		for (auto segment : snakeBody)
		{
			screenBuffer[g_ScreenWidth * segment.y + segment.x] = isDead ? L'+' : L'0'; 
		}
		//draw  snake`s head 
		screenBuffer[g_ScreenWidth * snakeBody.front().y + snakeBody.front().x] = isDead ? L'X' : L'@';


		//draw a food 
		screenBuffer[g_ScreenWidth * foodY + foodX] = L'$';
		//change color for food
		DWORD  coloredChars = 0;
		int foodColor = 10; 
		FillConsoleOutputAttribute(hConsole, foodColor, 1, { foodX,foodY }, &coloredChars); 


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
		

		//move snake
		switch (snakeDirection)
		{
		case 0: // up 
		{
			snakeBody.push_front({ snakeBody.front().x, snakeBody.front().y + 1 });
		}break;
		case 1: // right
		{
			snakeBody.push_front({ snakeBody.front().x - 1 , snakeBody.front().y});
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
		

		snakeBody.pop_back(); 



		//Display frame
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0, 0 }, &bytesWritten);
	}


	return 0; 
}