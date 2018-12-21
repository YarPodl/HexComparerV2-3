#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18
#define LENGTH_OF_BYTE 3

class AreaOfFile
{
private:
	INT				m_numberOfArea					= -1;
	FileCommander * m_fileCommander					= NULL;
	HWND			m_hWnd							= NULL;
	HINSTANCE		m_hInst							= NULL;
	RECT			m_rectData						= { 0 };
	RECT			m_rectMenu						= { 0 };
	WCHAR			m_format[LENGTH_OF_BUFFER]		= { 0 };
	HWND			m_hEdit							= NULL;
	HWND			m_hButton						= NULL;
	HWND			m_hScrollBar					= NULL;
	HFONT			m_hFont							= NULL;
	INT				m_indentForBytes				= 0;
	INT				m_indentForLetters				= 0;
	INT				m_widthChar						= 0;
	INT				m_heightChar					= 0;
	INT				m_lengthOfNumberRow				= 0;
	INT				m_countOfVisibleRows			= 0;
	INT64			m_scrollPos						= 0;

	// Соотношение между реальной позицией скроллинга и позицией бегунка
	double			m_ratioOfScroll					= 0;

	// Максимальная позиция бегунка
	INT				m_maxScrollPos					= 0;

	// Общее количество строк
	INT64			m_countRows						= 0;


	void		UpdateScrollInfo();
	void inline	PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView);
	void inline	PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte);
	void inline	ByteToHexString(byte in, WCHAR out[]);
	void		UpdateFont();
	void		UpdateNumberOfRow();


public:
	AreaOfFile();
	~AreaOfFile();

	BOOL		Initialize(INT number, HWND hWnd, HINSTANCE hInst, HFONT hFont, FileCommander * fileCommander);
	void		CloseHandle();
	void		Paint(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// Устанавливает новый размер области
	/// </summary>
	/// <param name="client">Содержит новый размер области</param>
	void		setSize(RECT client);
	//void		setFont(HFONT hFont);
	HWND		GetButton();
	HWND		GetEdit();
	HWND		GetScrollBar();
	INT			GetCountOfVisibleRows();
	void		SetData(INT64 countRows, double ratioOfScroll, INT maxScrollPos);
	void		Scroll(INT64 scrollInc);
};

