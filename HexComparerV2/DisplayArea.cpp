#include "stdafx.h"
#include "DisplayArea.h"

#define CALL_FOR_ALL_AREA(func) for (INT i = 0; i < COUNT_OF_FILES; i++) { m_areasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	this->m_hWnd = hWnd;
	this->m_hInst = hInst;

	////////////////////////////

	HDC hdc = GetDC(hWnd);
	m_hFont = CreateFontW(MAX_FONT_SIZE_HEIGHT, 
				MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION,
				0, 0, 
				FW_NORMAL, 
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,		
				DEFAULT_QUALITY, 
				FF_MODERN, 
				L"Times new Roman");

	SelectObject(hdc, m_hFont);


	
	ReleaseDC(hWnd, hdc);

	////////////////////////////////

	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_fileCommander);
	}
}


void DisplayArea::SetSizeAreaOfFile()
{
	// Координаты одной области файла
	RECT ClientRectFileArea = { 0 };

	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		ClientRectFileArea.top		= 0;
		ClientRectFileArea.left		= m_widthClient / COUNT_OF_FILES * i;	
		ClientRectFileArea.right	= m_widthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_heightClient;
		
		m_areasOfFiles[i].setSize(ClientRectFileArea);
		m_areasOfFiles[i].SetData(m_countRows, m_ratioOfScroll, m_maxScrollPos);

		
	}
}

void DisplayArea::UpdateData()
{
	INT64 CountOfBytes = m_fileCommander.getMaxSize();
	if (CountOfBytes != m_countOfByte)
	{
		m_countOfByte = m_fileCommander.getMaxSize();
		m_countRows = m_countOfByte / LENGTH_OF_BYTE_STRINGS + 1;
		m_maxScrollPos = m_countRows > MAXINT ? MAXINT : m_countRows;
		m_ratioOfScroll = m_countRows > MAXINT ? (double)m_countRows / m_maxScrollPos : 1;

		m_scrollPos = m_scrollPos > m_countRows ? m_maxScrollPos : m_scrollPos;

		for (INT i = 0; i < COUNT_OF_FILES; i++)
		{
			m_areasOfFiles[i].SetData(m_countRows, m_ratioOfScroll, m_maxScrollPos);
		}
		
		InvalidateRect(m_hWnd, NULL, TRUE);
		UpdateWindow(m_hWnd);
	}
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	m_heightClient = HIWORD(lParam);
	m_widthClient = LOWORD(lParam);

	SetSizeAreaOfFile();

	m_scrollInc = 0;
	Scroll();

	// Минимальное количество видимых строк 
	m_minCountOfVisibleRows = m_areasOfFiles[0].GetCountOfVisibleRows();

	// Цикл прохода по областям и поиска минимального значения для областей
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_minCountOfVisibleRows > m_areasOfFiles[i].GetCountOfVisibleRows())
		{
			m_minCountOfVisibleRows = m_areasOfFiles[i].GetCountOfVisibleRows();
		}
	}
	
}

LRESULT DisplayArea::Command(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{

	// Нажатие кнопки
	case BN_CLICKED:
		CALL_FOR_ALL_AREA(СlickButton(lParam));
		UpdateData();
		break;

	// Изменение поля
	case EN_CHANGE:
		CALL_FOR_ALL_AREA(СhangeEdit(lParam));
		UpdateData();
		break;

	// Обработка клавиш акселераторов
	case 1:
		switch (LOWORD(wParam))
		{
		case ID_BEGIN:
			scrollBegin();
			break;
		case ID_END:
			scrollEnd();
			break;
		case ID_PAGEUP:
			scrollPageUp(NULL);
			break;
		case ID_PAGEDOWN:
			scrollPageDown(NULL);
			break;
		case ID_LINEUP:
			scrollLineUp();
			break;
		case ID_LINEDOWN:
			scrollLineDown();
			break;
		case ID_ENTER:
			OpenFileFromEdit();
			break;

		default:
			break;
		}
	default:
		break;
	}

	return 0;
}

void DisplayArea::Paint(HDC hdc, PAINTSTRUCT &ps)
{
	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].PaintDump(hdc, ps);
	}
	SelectObject(hdc, m_hFont);
}

void DisplayArea::scrollLineUp()
{
	m_scrollInc = -1;
	Scroll();
}

void DisplayArea::scrollLineDown()
{
	m_scrollInc = 1;
	Scroll();
}

void DisplayArea::scrollPageUp(LPARAM lParam)
{
	m_scrollInc = -getCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::scrollPageDown(LPARAM lParam)
{
	m_scrollInc = getCountOfVisibleRows(lParam);
	Scroll();
}

void DisplayArea::scrollBegin()
{
	m_scrollInc = -m_scrollPos;
	Scroll();
}


void DisplayArea::scrollEnd()
{
	m_scrollInc = m_countRows;
	Scroll();
}

void DisplayArea::scrollTo(LPARAM lParam)
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].GetScrollBar() == (HWND)lParam)
		{
			SCROLLINFO ScrollInfo;
			ScrollInfo.cbSize = sizeof(ScrollInfo);
			ScrollInfo.fMask = SIF_TRACKPOS;
			if (!GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo))
			{
				INT t = GetLastError();
				t += 0;
			}
			//scrollPos = ratioOfScroll * scrollInfo.nTrackPos + 0.5;
			if (ScrollInfo.nTrackPos + m_areasOfFiles[i].GetCountOfVisibleRows() >= m_maxScrollPos)
			{
				scrollEnd();
			}
			else
			{
				m_scrollInc = m_ratioOfScroll * ScrollInfo.nTrackPos - m_scrollPos;
				Scroll();
				/*scrollPos = ratioOfScroll * scrollInfo.nTrackPos + 0.5;
				if ((maxScrollPos < 1000) || (scrollPos % 2 == 0))
				{
					Scroll();
				}*/
			}
		}
	}
}

void DisplayArea::Scroll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{

	case SB_BOTTOM:
		scrollEnd();
		break;

	case SB_TOP:
		scrollBegin();
		break;

	case SB_LINEUP:
		scrollLineUp();
		break;

	case SB_LINEDOWN:
		scrollLineDown();
		break;

	case SB_PAGEUP:
		scrollPageUp(lParam);
		break;

	case SB_PAGEDOWN:
		scrollPageDown(lParam);
		break;

	case SB_THUMBTRACK:
		scrollTo(lParam);
		break;

	case SB_THUMBPOSITION:
		scrollTo(lParam);
		break;

	default:
		break;
	}
}

bool DisplayArea::loadFile(INT indexFile, LPCWSTR fileName)
{
	if (!m_fileCommander.LoadFile(indexFile, fileName))
	{
		return FALSE;
	}

	UpdateData();

	return TRUE;
}


INT DisplayArea::getCountOfVisibleRows(LPARAM lParam)
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].GetScrollBar() == (HWND)lParam)
		{
			return m_areasOfFiles[i].GetCountOfVisibleRows();
		}
	}

	return m_minCountOfVisibleRows;
}

bool DisplayArea::OpenFileFromEdit()
{
	HWND FocusWindow = GetFocus();

	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].GetEdit() == FocusWindow)
		{
			m_areasOfFiles[i].OpenFile();
			UpdateData();
			return TRUE;
		}
	}

	return FALSE;
}


void DisplayArea::Scroll()
{
	m_scrollInc = max(
		-m_scrollPos,
		min(m_scrollInc, m_countRows - m_minCountOfVisibleRows - m_scrollPos)
	);
	if (m_scrollInc != 0)
	{
		m_scrollPos += m_scrollInc;
		for (INT i = 0; i < COUNT_OF_FILES; i++)
		{
			m_areasOfFiles[i].Scroll(m_scrollInc);
		}
	}
}

DisplayArea::~DisplayArea()
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_fileCommander.CloseFile(i);
	}
}
