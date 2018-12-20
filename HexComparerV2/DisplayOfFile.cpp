
#include "stdafx.h"
#include "DisplayOfFile.h"




DisplayOfFile::DisplayOfFile()
{
}

HWND DisplayOfFile::getButton()
{
	return hButton;
}

HWND DisplayOfFile::getEdit()
{
	return hEdit;
}


void DisplayOfFile::SetData(RECT clientRect, HWND hWnd, HINSTANCE hInst)
{
	this->hWnd = hWnd;
	this->hInst = hInst;
	rectData = clientRect;
	rectData.top = rectData.top + HEIGHT_MENU;
	rectData.bottom = rectData.bottom - HEIGHT_MENU;
	rectMenu = clientRect;
	rectData.bottom = HEIGHT_MENU;
	if (!hFont)
	{
		HDC hdc = GetDC(hWnd);
		hFont = CreateFontW(FONT_SIZE_HEIGHT, FONT_SIZE_WIDTH, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FIXED_PITCH, L"Arial");
		ReleaseDC(hWnd, hdc);
	}
	if (!hEdit)
	{
		hEdit = CreateWindowW(
			L"edit",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			rectMenu.left, rectMenu.top,
			rectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
			hWnd, nullptr, hInst, nullptr);
		SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, 1);
	}
	if (!hButton)
	{
		hButton = CreateWindowW(
			L"button",
			L">>",
			WS_CHILD | WS_VISIBLE,
			rectMenu.left + rectMenu.right - WIDTH_BUTTONS, rectMenu.top,
			WIDTH_BUTTONS, HEIGHT_BUTTONS,
			hWnd, nullptr, hInst, nullptr);
	}
	SetWindowPos(hEdit, HWND_BOTTOM,
		rectMenu.left, rectMenu.top,
		rectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);
	SetWindowPos(hButton, HWND_BOTTOM,
		rectMenu.left + rectMenu.right - WIDTH_BUTTONS, rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);
}
void DisplayOfFile::SetFont(int widthChar, int heightChar, INT64 countRows)
{
	this->widthChar = widthChar;
	this->heightChar = heightChar;
	lengthOfNumberRow = 1;
	while ((countRows /= 16) != 0)
	{
		lengthOfNumberRow++;
	}
	int temp = lengthOfNumberRow;
	wsprintfW(format, L"%%0%dlX:", temp);
	lengthOfNumberRow++;	// Двоеточие
	indentForBytes = rectData.left + widthChar * (INDENT1 + INDENT2 + lengthOfNumberRow);
	indentForLetters = rectData.left + widthChar * (INDENT1 + INDENT2 + INDENT3 + lengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2));

}

void DisplayOfFile::PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView)
{
	int lengthOfNumberRow = wsprintfW(buffer, format, numberLineForView);
	TextOutW(
		hdc, 
		rectData.left + widthChar * INDENT1,
		rectData.top + INDENT_OF_TOP + numberLine * heightChar,
		buffer,
		lengthOfNumberRow
	);
}

void DisplayOfFile::PaintByte(HDC hdc, int numberLine, int numberByte, byte value)
{
	int y = rectData.top + INDENT_OF_TOP + numberLine * heightChar;
	char charOfByte = value <= 31 ? '.' : value;
	TextOutA(
		hdc,
		indentForLetters + widthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);
	ByteToHexChars(value, buffer);
	TextOutW(
		hdc,
		indentForBytes + widthChar * (numberByte * (CharsForByte + INDENT_BETWEEN_BYTES2)),
		y,
		buffer,
		CharsForByte
	);
}

void DisplayOfFile::PaintFail(HDC hdc, int numberLine, int numberByte)
{
	int y = rectData.top + INDENT_OF_TOP + numberLine * heightChar;
	TextOutW(
		hdc,
		rectData.left + widthChar * (INDENT1 + INDENT2 + lengthOfNumberRow + numberByte * (CharsForByte + INDENT_BETWEEN_BYTES2)),
		y,
		L"--",
		CharsForByte
	); 
	TextOutA(
		hdc,
		indentForLetters + widthChar * numberByte,
		y,
		" ",
		1
	);
}



void DisplayOfFile::ByteToHexChars(byte in, WCHAR out[])
{
	out[0] = (in >> 4) + L'0';
	out[1] = (in & 0x0F) + L'0';
	out[3] = 0;
	if (out[0] > L'9')
	{
		out[0] = (out[0] - L'9' + L'A' - 1);
	}
	if (out[1] > L'9')
	{
		out[1] = (out[1] - L'9' + L'A' - 1);
	}
}


DisplayOfFile::~DisplayOfFile()
{
}
