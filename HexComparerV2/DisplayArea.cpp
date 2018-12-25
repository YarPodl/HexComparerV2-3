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

	// ���� ������� �� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// ������������� ��������
		m_AreasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_FileCommander);
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
		ClientRectFileArea.top		= 0;
		ClientRectFileArea.left		= m_WidthClient / COUNT_OF_FILES * i;
		ClientRectFileArea.right	= m_WidthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_HeightClient;
		
		// �������� ������� � ������ � ������
		m_AreasOfFiles[i].setSize(ClientRectFileArea);
		m_AreasOfFiles[i].SetData(m_CountRows, m_RatioOfScroll, m_MaxScrollPos);
	}
}


void DisplayArea::UpdateData()
{
	// ��������� ����� ������������ ���������� ����
	m_CountOfByte = m_FileCommander.GetMaxSize();

	// ������������� ��� ���������
	m_CountRows = m_CountOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_CountRows = m_CountOfByte == 0 ? 0 : m_CountRows;
	m_MaxScrollPos = m_CountRows > MAXINT ? MAXINT : m_CountRows;
	m_RatioOfScroll = m_CountRows > MAXINT ? (double)m_CountRows / m_MaxScrollPos : 1;
	m_ScrollPos = m_ScrollPos > m_CountRows ? m_MaxScrollPos : m_ScrollPos;

	// �������� ����� ��������� ���� ��������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_AreasOfFiles[i].SetData(m_CountRows, m_RatioOfScroll, m_MaxScrollPos);
	}
		
	// ������������ ���
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);

	UpdateMinCountOfVisibleRows();
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// ���������� ����� �������� ���������� �������
	m_HeightClient = HIWORD(lParam);
	m_WidthClient = LOWORD(lParam);

	// �������� ����� ������� ��������
	SetSizeAreaOfFile();

	// ���������� ���������� ������
	m_ScrollInc = 0;
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
	m_ScrollInc = -1;
	Scroll();
}


void DisplayArea::ScrollLineDown()
{
	m_ScrollInc = 1;
	Scroll();
}


void DisplayArea::ScrollPageUp(LPARAM lParam)
{
	m_ScrollInc = -GetCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::ScrollPageDown(LPARAM lParam)
{
	m_ScrollInc = GetCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::ScrollBegin()
{
	m_ScrollInc = -m_ScrollPos;
	Scroll();
}


void DisplayArea::ScrollEnd()
{
	m_ScrollInc = m_CountRows - m_ScrollPos - m_MinCountOfVisibleRows;
	Scroll();
}


void DisplayArea::ScrollTo(LPARAM lParam)
{
	SCROLLINFO	ScrollInfo;		// ���������� � �������

	// ��������� ���������� � �������
	ScrollInfo.cbSize = sizeof(ScrollInfo);
	ScrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo);

	// ���� ������� � ����� ������
	if (ScrollInfo.nTrackPos + m_MinCountOfVisibleRows + 1 >= m_MaxScrollPos)
	{
		ScrollEnd();
	}
	else
	{
		// ���������� ��������� ������ ������� �������
		m_ScrollInc = (INT64)(m_RatioOfScroll * ScrollInfo.nTrackPos + 0.5) - m_ScrollPos;

		Scroll();
	}
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
	m_ScrollInc = max(
		-m_ScrollPos,
		min(m_ScrollInc, m_CountRows - m_MinCountOfVisibleRows - m_ScrollPos)
	);

	// ���� ���������� ���������
	if (m_ScrollInc != 0)
	{
		// �������� �������
		m_ScrollPos += m_ScrollInc;

		// ��������� ������ ��� ������ �������
		CALL_FOR_ALL_AREA(Scroll(m_ScrollInc))
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
