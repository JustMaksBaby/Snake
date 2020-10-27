#include <Windows.h>
#include <list> 


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


	//snake body
	std::list<SnakeSegment> snake = { {60,15}, {61,15}, {62,15} };  


	//food position
	// on map can be only one food element 
	short foodX = 15; 
	short foodY = 13; 


	InitTopBar(screenBuffer);  
	//game loop 
	while (1)
	{
		//draw snake body
		for (auto segment : snake)
		{
			screenBuffer[g_ScreenWidth * segment.y + segment.x] = isDead ? L'+' : L'0'; 
		}
		//draw  snake`s head 
		screenBuffer[g_ScreenWidth * snake.front().y + snake.front().x] = isDead ? L'X' : L'@';


		//draw a food 
		screenBuffer[g_ScreenWidth * foodY + foodX] = L'$';
		//change color for food
		DWORD  coloredChars = 0;
		FillConsoleOutputAttribute(hConsole, 10, 1, { foodX,foodY }, &coloredChars);


		int a = 2.3f; 
		//Display frame
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0, 0 }, &bytesWritten);
	}


	return 0; 
}