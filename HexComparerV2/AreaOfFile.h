#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18
#define LENGTH_OF_BYTE 3

class AreaOfFile
{
private:
	int				m_numberOfArea					= -1;
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
	int				m_indentForBytes				= 0;
	int				m_indentForLetters				= 0;
	int				m_widthChar						= 0;
	int				m_heightChar					= 0;
	int				m_lengthOfNumberRow				= 0;
	int				m_countOfVisibleRows			= 0;
	INT64			m_scrollPos						= 0;

	// ����������� ����� �������� �������� ���������� � �������� �������
	double			m_ratioOfScroll					= 0;

	// ������������ ������� �������
	int				m_maxScrollPos					= 0;

	// ����� ���������� �����
	INT64			m_countRows						= 0;


	void		UpdateScrollInfo();
	void inline	PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView);
	void inline	PaintByte(HDC hdc, int numberLine, int numberByte, WCHAR stringOfByte[], char charOfByte);
	void inline	ByteToHexString(byte in, WCHAR out[]);
	void		UpdateFont();
	void		UpdateNumberOfRow();


public:
	AreaOfFile();
	~AreaOfFile();

	BOOL		Initialize(int number, HWND hWnd, HINSTANCE hInst, FileCommander * fileCommander);
	void		CloseHandle();
	void		Paint(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// ������������� ����� ������ �������
	/// </summary>
	/// <param name="client">�������� ����� ������ �������</param>
	void		setSize(RECT client);
	//void		setFont(HFONT hFont);
	HWND		getButton();
	HWND		getEdit();
	HWND		getScrollBar();
	int			getCountOfVisibleRows();
	void		setDateOfScroll(INT64 countRows, double ratioOfScroll, int maxScrollPos);
	void		Scroll(INT64 scrollInc);
};

