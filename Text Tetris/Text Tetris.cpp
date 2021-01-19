#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>

using namespace std;

#define pii pair<int, int>

const int WIDTH = 12;
const int HEIGHT = 18;

const int screenWIDTH = 120;
const int screenHEIGHT = 30;

unsigned char* pField = nullptr;

wstring tetromino[7];

void createTetromino()
{
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L".XX.");
	tetromino[3].append(L"..X.");
	tetromino[3].append(L"..X.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"....");

	tetromino[6].append(L"..X.");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L"..X.");
	tetromino[6].append(L"....");
}

void drawFrame(wchar_t *screen)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			screen[(y + 2) * screenWIDTH + (x + 2)] = L" ABCDEFG=#"[pField[y * WIDTH + x]];
		}
	}
}

int Rotate(int x, int y, int r)
{
	switch (r % 4)
	{
		case 0:
			return y * 4 + x;
		case 1:
			return 12 + y - x * 4;
		case 2:
			return 15 - y * 4 - x;
		case 3:
			return 3 - y + x * 4;
	}
}

bool Valid(int px, int py, int type, int r)
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			int pi = Rotate(x, y, r);

			int fi = (py + y) * WIDTH + (px + x);
			if (px + x >= 0 && px + x < WIDTH && py + y >= 0 && py + y < HEIGHT)
			{
				// Hits something else
				if (tetromino[type][pi] != L'.' && pField[fi] != 0)
					return 0;
			}
		}
	}

	return 1;
}

int main()
{
	createTetromino();
	pField = new unsigned char[WIDTH * HEIGHT];
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			pField[y * WIDTH + x] = (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? 9 : 0;
		}
	}

	wchar_t *screen = new wchar_t[screenWIDTH * screenHEIGHT];
	for (int i = 0; i < screenWIDTH * screenHEIGHT; i++)
	{
		screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	int piece = 0;
	int rotation = rand() % 7;
	int X = WIDTH / 2;
	int Y = 0;

	bool bKey[4];
	bool iRot = 0;

	int speed = 18;
	int speedMin = 2;
	int speedCounter = 0;
	int totalTime = 0;
	bool autoDown = 0;
	int score = 0;

	vector<int> lines;

	bool gameover = 0;
	while (!gameover)
	{
		// TIMER ======================================================================================================

		this_thread::sleep_for(40ms);
		autoDown = (++speedCounter == max(speed - totalTime / 10000, speedMin));
		totalTime += 40;

		// INPUT ======================================================================================================

		for (int k = 0; k < 4; k++)
		{
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k])));
		}

		// LOGIC ======================================================================================================
		X += (bKey[0] && Valid(X + 1, Y, piece, rotation) ? 1 : 0);
		X -= (bKey[1] && Valid(X - 1, Y, piece, rotation) ? 1 : 0);
		Y += (bKey[2] && Valid(X, Y + 1, piece, rotation) ? 1 : 0);
		if (!iRot)
		{
			rotation += (bKey[3] && Valid(X, Y, piece, rotation + 1) ? 1 : 0);
			iRot = 1;
		}
		else
			iRot = 0;

		if (autoDown)
		{
			speedCounter = 0;

			if (Valid(X, Y + 1, piece, rotation))
				Y++;
			else
			{
				// Lock piece
				for (int x = 0; x < 4; x++)
				{
					for (int y = 0; y < 4; y++)
					{
						if (tetromino[piece][Rotate(x, y, rotation)] == L'X')
							pField[(Y + y) * WIDTH + (X + x)] = piece + 1;
					}
				}
				// Check lines
				for (int y = 0; y < 4; y++)
				{
					if (Y + y < HEIGHT - 1)
					{
						bool line = 1;
						for (int x = 1; x < WIDTH - 1; x++)
							line &= (pField[(Y + y) * WIDTH + x]) != 0;

						if (line) 
						{
							for (int x = 1; x < WIDTH - 1; x++)
								pField[(Y + y) * WIDTH + x] = 8;
							lines.push_back(Y + y);
						}
					}
				}

				if (!lines.empty())
					score += (1 << (lines.size() - 1)) * 50;

				// Choose next piece
				X = WIDTH / 2;
				Y = 0;
				rotation = 0;
				piece = rand() % 7;

				// Gameover?
				gameover = !Valid(X, Y, piece, rotation);
			}
		}

		// OUTPUT =====================================================================================================

		drawFrame(screen);

		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				if (tetromino[piece][Rotate(x, y, rotation)] == L'X')
					screen[(Y + y + 2) * screenWIDTH + (X + x + 2)] = piece + 65;
			}
		}

		if (!lines.empty())
		{
			WriteConsoleOutputCharacter(hConsole, screen, screenWIDTH * screenHEIGHT, { 0, 0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);

			for (auto& line : lines)
			{
				for (int x = 1; x < WIDTH - 1; x++)
				{
					for (int y = line; y > 0; y--)
						pField[y * WIDTH + x] = pField[(y - 1) * WIDTH + x];
//					pField[x] = 0;
				}
			}
			lines.clear();
		}

		// Display frame
		WriteConsoleOutputCharacter(hConsole, screen, screenWIDTH * screenHEIGHT, { 0, 0 }, &dwBytesWritten);

		swprintf_s(&screen[2 * screenWIDTH + WIDTH + 6], 16, L"SCORE: %8d", score);

	}

	CloseHandle(hConsole);
	cout << "Game Over!! Score: " << score << endl;
	system("pause");

	return 0;
}