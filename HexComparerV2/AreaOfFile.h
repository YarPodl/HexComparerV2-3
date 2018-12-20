#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "DisplayOfFile.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18
#define LENGTH_OF_BYTE 3

class AreaOfFile
{
private:
	int				m_numberOfArea					= 0;
	FileCommander * m_fileCommander					= NULL;
	HWND			hWnd							= NULL;
	HWND			m_scrollBar						= NULL;
	HINSTANCE		hInst							= NULL;
	RECT			m_rectData						= { 0 };
	RECT			rectMenu						= { 0 };
	WCHAR			m_buffer[LENGTH_OF_BUFFER]		= { 0 };
	char			cBuffer[LENGTH_OF_BYTE]			= { 0 };
	WCHAR			format[LENGTH_OF_BUFFER]		= { 0 };
	HWND			m_hEdit							= NULL;
	HWND			m_hButton						= NULL;
	HFONT			m_hFont							= NULL;
	int				indentForBytes					= 0;
	int				indentForLetters				= 0;
	int				widthChar						= 0;
	int				heightChar						= 0;
	int				m_lengthOfNumberRow				= 0;
	INT64			m_countOfVisibleRows			= 0;
	INT64			m_scrollPos						= 0;
	COLORREF		m_baseTextColor					= 0;




	void UpdateScrollInfo();
	void inline	PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView);
	void inline	PaintByte(HDC hdc, int numberLine, int numberByte, WCHAR stringOfByte[], char charOfByte);
	void inline	ByteToHexString(byte in, WCHAR out[]);


public:
	AreaOfFile();
	~AreaOfFile();
	void		Paint(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// Устанавливает новый размер области
	/// </summary>
	/// <param name="client"></param>
	void		setSize(RECT client);
	void		setFont(HFONT hFont);
	void		setFileCommander(FileCommander * fileCommander);
	HWND		getButton();
	HWND		getEdit();
	HWND		getScrollBar();
	int			getCountOfVisibleRows();
	void		Scroll(INT64 scrollInc);
};

