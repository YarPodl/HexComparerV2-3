#pragma once
#include "stdafx.h"

#define LENGTH_OF_BUFFER 18
#define LENGTH_OF_BYTE 3


class DisplayOfFile
{
private:

	HWND		hWnd;
	HINSTANCE	hInst;
	RECT		rectData;
	RECT		rectMenu;
	WCHAR		buffer[LENGTH_OF_BUFFER];
	char		cBuffer[LENGTH_OF_BYTE];
	WCHAR		format[LENGTH_OF_BUFFER];
	HWND		hEdit						= NULL;
	HWND		hButton						= NULL;
	HFONT		hFont						= NULL;
	int			indentForBytes;
	int			indentForLetters;
	int			widthChar;
	int			heightChar;

	int			lengthOfNumberRow;

	void inline ByteToHexChars(byte in, WCHAR out[]);
public:
	DisplayOfFile();
	HWND getButton();
	HWND getEdit();
	void SetData(RECT clientRect, HWND hWnd, HINSTANCE hInst);
	void SetFont(int widthChar, int heightChar, INT64 countRows);
	void PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView);
	void PaintByte(HDC hdc, int numberLine, int numberByte, byte value);
	void PaintFail(HDC hdc, int numberLine, int numberByte);
	~DisplayOfFile();
};

