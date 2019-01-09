#include "stdafx.h"
#include "DisplayArea.h"

// ����� ������ �� ��������� ��� ������� �������� ������� m_AreasOfFiles
#define CALL_FOR_ALL_AREA(func) for (DWORD i = 0; i < COUNT_OF_FILES; i++) { m_AreasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	// ����������� ������������
	this->m_hWnd = hWnd;
	this->m_hInst = hInst;

	// �������� ��������� ������
	HDC hdc = GetDC(hWnd);
	m_hFont = CreateFontW(MAX_FONT_SIZE_HEIGHT, 
				(int)(MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION),
				0, 0, 
				FW_NORMAL, 
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,		
				DEFAULT_QUALITY, 
				FF_MODERN, 
				L"Times new Roman");
	ReleaseDC(hWnd, hdc);


	// ������ ������ ���������� �������
	m_hButtonNext = CreateWindowW(
		L"button",
		L"��������� �������",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// ������ ������ ����������� �������
	m_hButtonPrev = CreateWindowW(
		L"button",
		L"���������� �������",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 2 + WIDTH_BUTTONS, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// ������ ������ ���������� �������
	m_hButtonSearch = CreateWindowW(
		L"button",
		L"��������",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 3 + WIDTH_BUTTONS * 2, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// ����� ��� ������
	SendMessageW(m_hButtonNext, WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonPrev, WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonSearch, WM_SETFONT, (WPARAM)m_hFont, 1);

	// ���� ������� �� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// ������������� ��������
		m_AreasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_FileCommander, &m_DataOfScroll);
	}
}


void DisplayArea::SetSizeAreaOfFile()
{
	RECT	ClientRectFileArea = { 0 };		// ���������� ����� ������� �����
	INT		i;								// ���������� �����

	// ���� ������� �� ��������
	for (i = 0; i < COUNT_OF_FILES; i++)
	{
		// ���������� �������
		ClientRectFileArea.top		= HEIGHT_BUTTONS * 1.5;
		ClientRectFileArea.left		= m_WidthClient / COUNT_OF_FILES * i;
		ClientRectFileArea.right	= m_WidthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_HeightClient;
		
		// �������� ������� � ������ � ������
		m_AreasOfFiles[i].setSize(ClientRectFileArea);
		m_AreasOfFiles[i].UpdateData();
	}
}


void DisplayArea::UpdateData()
{
	// ��������� ����� ������������ ���������� ����
	m_CountOfByte = m_FileCommander.GetMaxSize();

	// ������������� ��� ���������
	m_DataOfScroll.CountRows		= m_CountOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_DataOfScroll.CountRows		= m_CountOfByte == 0 ? 0 : m_DataOfScroll.CountRows;
	m_DataOfScroll.MaxScrollPos		= m_DataOfScroll.CountRows > MAXINT ? MAXINT : m_DataOfScroll.CountRows;
	m_DataOfScroll.RatioOfScroll	= m_DataOfScroll.CountRows > MAXINT
											? (double)m_DataOfScroll.CountRows / m_DataOfScroll.MaxScrollPos
											: 1;
	m_DataOfScroll.ScrollPos		= m_DataOfScroll.ScrollPos > m_DataOfScroll.CountRows
											? m_DataOfScroll.MaxScrollPos
											: m_DataOfScroll.ScrollPos;

	// �������� ����� ��������� ���� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_AreasOfFiles[i].UpdateData();
	}

	UpdateMinCountOfVisibleRows();


	// ���� ������� ������� ������� ����� �� �������, ������� �� � ������
	if (m_DataOfScroll.ScrollPos > m_DataOfScroll.CountRows - m_MinCountOfVisibleRows)
	{
		m_DataOfScroll.ScrollPos = 0;
	}

	// ������������ ���
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// ���������� ����� �������� ���������� �������
	m_HeightClient = HIWORD(lParam);
	m_WidthClient = LOWORD(lParam);

	// �������� ����� ������� ��������
	SetSizeAreaOfFile();

	// ���������� ���������� ������
	m_DataOfScroll.ScrollInc = 0;
	Scroll();

	// ����������� ���������� ������� ����� 
	UpdateMinCountOfVisibleRows();
}


void DisplayArea::Paint(HDC hdc, PAINTSTRUCT &ps)
{
	// ���� ������� �� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// ��������� ������ �������
		m_AreasOfFiles[i].PaintArea(hdc, ps);
	}

	// ������� �������� ������
	SelectObject(hdc, m_hFont);
}


void DisplayArea::ScrollLineUp()
{
	m_DataOfScroll.ScrollInc = -1;
	Scroll();
}


void DisplayArea::ScrollLineDown()
{
	m_DataOfScroll.ScrollInc = 1;
	Scroll();
}


void DisplayArea::ScrollPageUp(LPARAM lParam)
{
	m_DataOfScroll.ScrollInc = -GetCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::ScrollPageDown(LPARAM lParam)
{
	m_DataOfScroll.ScrollInc = GetCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::ScrollBegin()
{
	m_DataOfScroll.ScrollInc = -m_DataOfScroll.ScrollPos;
	Scroll();
}


void DisplayArea::ScrollEnd()
{
	m_DataOfScroll.ScrollInc = m_DataOfScroll.CountRows - m_DataOfScroll.ScrollPos - m_MinCountOfVisibleRows;
	Scroll();
}


void DisplayArea::ScrollTo(LPARAM lParam)
{
	SCROLLINFO	ScrollInfo;		// ���������� � �������

	// ��������� ���������� � �������
	ScrollInfo.cbSize = sizeof(ScrollInfo);
	ScrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo);

	// ���������� ��������� ������ ������� �������
	m_DataOfScroll.ScrollInc = (INT64)
		(m_DataOfScroll.RatioOfScroll * ScrollInfo.nTrackPos + 0.5) - m_DataOfScroll.ScrollPos;

	Scroll();
}



INT DisplayArea::GetCountOfVisibleRows(LPARAM lParam)
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_AreasOfFiles[i].GetScrollBar() == (HWND)lParam)
		{
			return m_AreasOfFiles[i].GetCountOfVisibleRows();
		}
	}

	return m_MinCountOfVisibleRows;
}

void DisplayArea::UpdateMinCountOfVisibleRows()
{
	// ����������� ���������� ������� ����� 
	INT MinCountOfVisibleRows = MAXINT;

	// ���� ������� �� �������� � ������ ������������ �������� ��� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// ����������� ������ ������� ������� ����������� ����
		if ((m_FileCommander.IsLoadedFile(i)) && (MinCountOfVisibleRows > m_AreasOfFiles[i].GetCountOfVisibleRows()))
		{
			MinCountOfVisibleRows = m_AreasOfFiles[i].GetCountOfVisibleRows();
		}
	}

	// ���� �� ���� ���� �� ��������
	if (MinCountOfVisibleRows == MAXINT)
	{
		MinCountOfVisibleRows = m_AreasOfFiles[0].GetCountOfVisibleRows();
	}

	m_MinCountOfVisibleRows = MinCountOfVisibleRows;
}


bool DisplayArea::OpenFileFromEdit()
{
	HWND FocusWindow = GetFocus();	// ����, ����������� �����

	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// ���� ����� � ���� ��������������
		if (m_AreasOfFiles[i].GetEdit() == FocusWindow)
		{
			// ������� ���� �� ������� ����
			m_AreasOfFiles[i].OpenFile();

			// ����������� ���������
			UpdateData();

			return TRUE;
		}
	}

	return FALSE;
}


void DisplayArea::�hangeEdit(HWND hEdit)
{
	CALL_FOR_ALL_AREA(�hangeEdit(hEdit));
	UpdateData();
}


void DisplayArea::�lickButton(HWND hButton)
{

	CALL_FOR_ALL_AREA(�lickButton(hButton));
	UpdateData();
}


void DisplayArea::Scroll()
{
	// �������� �� ����� �� �������
	m_DataOfScroll.ScrollInc = max(
		-m_DataOfScroll.ScrollPos,
		min(m_DataOfScroll.ScrollInc, m_DataOfScroll.CountRows - m_MinCountOfVisibleRows - m_DataOfScroll.ScrollPos)
	);

	// ���� ���������� ���������
	if (m_DataOfScroll.ScrollInc != 0)
	{
		// �������� �������
		m_DataOfScroll.ScrollPos += m_DataOfScroll.ScrollInc;

		// ��������� ������ ��� ������ �������
		CALL_FOR_ALL_AREA(Scroll())
	}
}


DisplayArea::~DisplayArea()
{
	// ������� ��� �����
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_FileCommander.CloseFile(i);
	}
}
