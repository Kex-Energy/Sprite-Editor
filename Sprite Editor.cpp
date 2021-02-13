// Sprite Editor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

int nScreenWidth = 120;				// Console Screen Size X (columns)
int nScreenHeight = 30;				// Console Screen Size Y (rows)
int const nSpriteWidth = 30;
int const nSpriteHeight = 30;

class Sprite
{
public:
    _CHAR_INFO* pixels;
    int Height, Width;

    Sprite(int Height, int Width) {
        this->Height = Height;
        this->Width = Width;

        delete[] pixels;
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
    wchar_t* screen_buffer = new wchar_t[nScreenWidth * nScreenHeight];

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        colors[i] = 0x07;
        screen_buffer[i] = L' ';
    }

    unsigned int mode = 0;// 0 - new; 1 - edit
    bool selected = false;
    bool ended = false;
    
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
    WriteConsoleOutputAttribute(hConsole, colors, nScreenWidth * nScreenHeight, { 0,0 }, &OUTP);
    WriteConsoleOutputCharacter(hConsole, screen_buffer, nScreenHeight*nScreenWidth, { 0,0 }, &OUTP);

    nScreenWidth = nSpriteWidth < 120 ? 120 : nSpriteWidth;
    nScreenHeight = nSpriteHeight < 120 ? 120 : nSpriteHeight;
    
    delete[] screen_buffer;
    delete[] colors;

    screen_buffer = new wchar_t[nScreenWidth * nScreenHeight];
    colors = new unsigned short[nScreenWidth * nScreenHeight];

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    {
        colors[i] = 0x07;
        screen_buffer[i] = L' ';
    }

    SetConsoleScreenBufferSize(hConsole, { (short)nScreenWidth,(short)nScreenHeight });

    font.nFont = 0;
    font.dwFontSize.X = 10;
    font.dwFontSize.Y = 10;
    font.FontFamily = FF_DONTCARE;
    font.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(hConsole, false, &font);

    rectWindow = { 0,0,(short)nScreenWidth - 1,(short)nScreenHeight - 1 };
    SetConsoleWindowInfo(hConsole, true, &rectWindow);

    unsigned short selected_color_back = 0x00, selected_color_char = 0x00, selected_block = 0;

    wchar_t blocks[4] = { L'█',L'▓',L'▒',L'░' };
    while (!ended) 
    {

        if (GetAsyncKeyState(VK_SUBTRACT) & 0x0001)
        {
            selected_color_back += 0x10;
        }

        if (GetAsyncKeyState(VK_ADD) & 0x0001)
        {
            selected_color_back += 0x10;
        }

        if (GetAsyncKeyState(VK_UP) & 0x0001)
        {
            selected_color_char += 0x01;
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x0001)
        {
            selected_color_char += 0x01;
        }

        if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
        {
            selected_block++;
        }

        if (GetAsyncKeyState(VK_LEFT) & 0x0001)
        {
            selected_block++;
        }

        selected_color_back %= 0x100;
        selected_color_char %= 0x10;
        selected_block %= 4;

        for (int i = 0; i < nScreenWidth * 6; i++)
        {
            colors[i] = 0xFA;
        }
        colors[2 * nScreenWidth + 10] = 0x0F | selected_color_back;
        colors[3 * nScreenWidth + 10] = 0x0F | selected_color_back;
        colors[2 * nScreenWidth + 11] = 0x0F | selected_color_back;
        colors[3 * nScreenWidth + 11] = 0x0F | selected_color_back;

        colors[2 * nScreenWidth + 14] = 0x00 | selected_color_char;
        colors[3 * nScreenWidth + 14] = 0x00 | selected_color_char;
        colors[2 * nScreenWidth + 15] = 0x00 | selected_color_char;
        colors[3 * nScreenWidth + 15] = 0x00 | selected_color_char;

        screen_buffer[2 * nScreenWidth + 14] = blocks[selected_block];
        screen_buffer[3 * nScreenWidth + 14] = blocks[selected_block];
        screen_buffer[2 * nScreenWidth + 15] = blocks[selected_block];
        screen_buffer[3 * nScreenWidth + 15] = blocks[selected_block];




        WriteConsoleOutputAttribute(hConsole, colors, nScreenWidth * nScreenHeight, { 0,0 }, &OUTP);
        WriteConsoleOutputCharacter(hConsole, screen_buffer, nScreenHeight * nScreenWidth, { 0,0 }, &OUTP);
    }
}


