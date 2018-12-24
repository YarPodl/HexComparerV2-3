#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18
#define LENGTH_OF_BYTE 3

class AreaOfFile
{
private:
	INT				m_NumberOfArea					= -1;
	FileCommander * m_pFileCommander				= NULL;
	HWND			m_hWnd							= NULL;
	HINSTANCE		m_hInst							= NULL;
	RECT			m_RectData						= { 0 };
	RECT			m_RectMenu						= { 0 };
	WCHAR			m_Format[LENGTH_OF_BUFFER]		= { 0 };
	HWND			m_hEdit							= NULL;
	HWND			m_hButton						= NULL;
	HWND			m_hScrollBar					= NULL;
	HFONT			m_hFont							= NULL;
	INT				m_IndentForBytes				= 0;
	INT				m_IndentForLetters				= 0;
	INT				m_WidthChar						= 0;
	INT				m_HeightChar					= 0;
	INT				m_LengthOfNumberRow				= 0;
	INT				m_CountOfVisibleRows			= 0;
	INT64			m_ScrollPos						= 0;

	// Соотношение между реальной позицией скроллинга и позицией бегунка
	double			m_RatioOfScroll					= 0;

	// Максимальная позиция бегунка
	INT				m_MaxScrollPos					= 0;

	// Общее количество строк
	INT64			m_CountRows						= 0;


	void		UpdateScrollInfo();
	void inline	PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView);
	void inline	PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte);
	void inline	ByteToHexString(byte in, WCHAR out[]);
	void		UpdateFont();
	void		UpdateNumberOfRow();
	bool		OpenFileDialog(LPWSTR file);


public:
	AreaOfFile();
	~AreaOfFile();

	BOOL		Initialize(INT number, HWND hWnd, HINSTANCE hInst, HFONT hFont, FileCommander * fileCommander);
	void		CloseHandle();
	void		PaintDump(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// Устанавливает новый размер области
	/// </summary>
	/// <param name="client">Содержит новый размер области</param>
	void		setSize(RECT client);
	HWND		GetButton();
	HWND		GetEdit();
	HWND		GetScrollBar();
	INT			GetCountOfVisibleRows();
	void		SetData(INT64 countRows, double ratioOfScroll, INT maxScrollPos);
	void		Scroll(INT64 scrollInc);
	void		СhangeEdit(LPARAM lParam);
	void		СlickButton(LPARAM lParam);
	void		OpenFile();
};

