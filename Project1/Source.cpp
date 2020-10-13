#include <Windows.h>


//in characters
static const int g_ScreenWidth  = 120; 
static const int g_ScreenHeight = 80;  





int main()
{

	//create console buffer 
	wchar_t * screenBuffer = new wchar_t[g_ScreenHeight * g_ScreenWidth]; 
	memset(screenBuffer, ' ', g_ScreenHeight * g_ScreenWidth); 

	//set console buffer 
	HANDLE  hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleActiveScreenBuffer(hConsole); 

	DWORD bytesWritten = 0; 


	while (1)
	{




		//Display frame
		WriteConsoleOutputCharacterW(hConsole, screenBuffer, g_ScreenHeight * g_ScreenWidth, { 0,0 }, &bytesWritten); 
	}


	return 0; 
}