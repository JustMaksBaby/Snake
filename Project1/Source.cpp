#include <Windows.h>
#include <list> 


#pragma comment (lib, "User32.lib")

//in characters
static const int g_ScreenWidth  = 100; 
static const int g_ScreenHeight = 60;  


struct SnakeSegment
{
	//position in chars 
	int x; 
	int y; 
};


void  InitTopBar(wchar_t * buffer)
{
	for (int i = 0; i < g_ScreenWidth; ++i)
	{
		buffer[i] = L'='; 
		buffer[2 * g_ScreenWidth + i] = L'=';
	}

	wsprintfW(buffer + g_ScreenWidth + 55, L"S.N.A.K.E"); 
}

int main()
{
	//create console buffer
	constexpr int BUFFER_SIZE = g_ScreenHeight * g_ScreenWidth;
	wchar_t* screenBuffer = new wchar_t[BUFFER_SIZE];
	//init  map
	//for (int i = 0; i < BUFFER_SIZE; ++i)
	//{
	//	screenBuffer[i] = L' '; 
	//}


	//set console buffer 
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleActiveScreenBuffer(hConsole); 

	//count chars written in console
	DWORD bytesWritten = 0; 

	//GENERAL INFO
	bool isDead = false; 
	//count foods that we have eaten
	int  score = 0; 


	//snake budy
	std::list<SnakeSegment> snake = { {60,15}, {61,15}, {62,15} };  


	//food position
	// on map can be only one food element 
	int foodX = 15; 
	int foodY = 13; 


	InitTopBar(screenBuffer);  

	//game loop 
	while (1)
	{
		//draw snake budy
		for (auto segment : snake)
		{
			screenBuffer[g_ScreenWidth * segment.y + segment.x] = isDead ? L'+' : L'0'; 
		}
		//draw  snake`s head 
		screenBuffer[g_ScreenWidth * snake.front().y + snake.front().x] = isDead ? L'X' : L'@';


		//draw a food 
		screenBuffer[g_ScreenWidth * foodY + foodX] = L'$';



		//Display frame
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0, 0 }, &bytesWritten);
	}


	return 0; 
}