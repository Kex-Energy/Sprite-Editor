// Sprite Editor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

int const nScreenWidth = 120;				// Console Screen Size X (columns)
int const nScreenHeight = 30;				// Console Screen Size Y (rows)

class Sprite
{
public:
    _CHAR_INFO* pixels;
    int Height, Width;

    Sprite(int Height, int Width) {
        this->Height = Height;
        this->Width = Width;

        delete pixels;
        pixels = new _CHAR_INFO[Width * Height];

        for (int i = 0; i < Width * Height; i++) 
        {
            pixels[i].Attributes = 0;
            pixels[i].Char.UnicodeChar = L' ';
        }
    }
};

int main()
{
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    DWORD OUTP = 0;
    SetConsoleActiveScreenBuffer(hConsole);
    SetConsoleScreenBufferSize(hConsole, { (short)nScreenWidth,(short)nScreenHeight });
    CONSOLE_FONT_INFOEX font = {};
    GetCurrentConsoleFontEx(hConsole, false, &font);
    font.dwFontSize = { 8,16 };
    font.cbSize = sizeof(font);
    SetCurrentConsoleFontEx(hConsole, false, &font);
    SMALL_RECT rectWindow;
    rectWindow = { 0,0,(short)nScreenWidth - 1,(short)nScreenHeight - 1 };
    SetConsoleWindowInfo(hConsole, true, &rectWindow);

    unsigned short* colors = new unsigned short[nScreenWidth * nScreenHeight];
    wchar_t* screen_buffer = new wchar_t[(int)nScreenWidth * (int)nScreenHeight];

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        colors[i] = 0x07;
        screen_buffer[i] = L' ';
    }

    unsigned int mode = 0;// 0 - new; 1 - edit
    bool selected = false;
    
    WriteConsoleOutputCharacter(hConsole, L"Create New", 10, { 0,0 }, &OUTP);
    WriteConsoleOutputCharacter(hConsole, L"Edit", 4, { 0,1 }, &OUTP);

    while (!selected) 
    {
        if (GetAsyncKeyState(VK_UP) & 0x0001)
        {
            mode--;
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x0001)
        {
            mode++;
        }
        
        mode %= 2;
        if (mode == 0) 
        {
            for (int x = 0; x < 10; x++)
                colors[x] = 0x70;
        }
        if (mode == 1)
        {
            for (int x = 0; x < 4; x++)
                colors[nScreenWidth + x] = 0x70;
        }
        WriteConsoleOutputAttribute(hConsole, colors, nScreenWidth * nScreenHeight, { 0,0 }, &OUTP);
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        {
            colors[i] = 0x07;
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x0001)
        {
            selected = true;
        }

    }
    
    wchar_t input[11];
    CONSOLE_READCONSOLE_CONTROL test;
    test.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
    test.nInitialChars = 0;
    test.dwCtrlWakeupMask = (1 << L'\n');
    test.dwControlKeyState = NULL;
    WriteConsoleOutputCharacter(hConsole, screen_buffer, nScreenHeight*nScreenWidth, { 0,0 }, &OUTP);
    while(mode == 0)
    {
        ReadConsole(hConsole, input, MAX_PATH * sizeof(TCHAR), &OUTP, &test);
        if (GetAsyncKeyState(VK_RETURN) & 0x0001)
        {
            mode = 1;
        }
    }
    system("pause");
}


