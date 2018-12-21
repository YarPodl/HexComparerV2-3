#include "stdafx.h"
#include "DisplayArea.h"

#define CALL_FOR_ALL_AREA(func) for (INT i = 0; i < COUNT_OF_FILES; i++) { m_areasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	this->hWnd = hWnd;
	this->hInst = hInst;

	////////////////////////////

	HDC hdc = GetDC(hWnd);
	hFont = CreateFontW(MAX_FONT_SIZE_HEIGHT, 
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

	SelectObject(hdc, hFont);


	
	ReleaseDC(hWnd, hdc);

	////////////////////////////////

	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Initialize(i, hWnd, hInst, hFont, &m_fileCommander);
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
		ClientRectFileArea.left		= widthClient / COUNT_OF_FILES * i;	
		ClientRectFileArea.right	= widthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= heightClient;
		
		m_areasOfFiles[i].setSize(ClientRectFileArea);
		m_areasOfFiles[i].SetData(m_countRows, m_ratioOfScroll, m_maxScrollPos);

		
	}
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	heightClient = HIWORD(lParam);
	widthClient = LOWORD(lParam);

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

void DisplayArea::Command(WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{

	// Нажатие кнопки
	case BN_CLICKED:
		СlickButton(lParam);
		break;

	// Изменение поля
	case EN_CHANGE:
		СhangeEdit(lParam);
		break;

	default:
		break;
	}
}

void DisplayArea::Paint(HDC hdc, PAINTSTRUCT &ps)
{
	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Paint(hdc, ps);
	}
	SelectObject(hdc, hFont);
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
	m_scrollInc -= getCountOfVisibleRows(lParam);
	Scroll();
}


void DisplayArea::scrollPageDown(LPARAM lParam)
{
	m_scrollInc += getCountOfVisibleRows(lParam);
	Scroll();
}

void DisplayArea::scrollBegin()
{
	m_scrollInc = -scrollPos;
	Scroll();
}


void DisplayArea::scrollEnd()
{
	m_scrollInc = m_countRows - scrollPos - m_minCountOfVisibleRows;
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
				m_scrollInc = m_ratioOfScroll * ScrollInfo.nTrackPos - scrollPos;
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

bool DisplayArea::loadFile(INT indexFile, LPCWSTR fileName)
{
	if (!m_fileCommander.LoadFile(indexFile, fileName))
	{
		return FALSE;
	}
	countOfByte = m_fileCommander.getMaxSize();
	m_countRows = countOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_maxScrollPos = m_countRows > MAXINT ? MAXINT : m_countRows;
	m_ratioOfScroll = m_countRows > MAXINT ? (double)m_countRows / m_maxScrollPos : 1;

	scrollPos = scrollPos > m_countRows ? m_maxScrollPos : scrollPos;


	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].SetData(m_countRows, m_ratioOfScroll, m_maxScrollPos);
	}

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

	return 0;
}

void DisplayArea::СhangeEdit(LPARAM lParam)
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].GetEdit() == (HWND)lParam)
		{
			m_fileCommander.CloseFile(i);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		}
	}
}

void DisplayArea::СlickButton(LPARAM lParam)
{
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].GetButton() == (HWND)lParam)
		{
			WCHAR buffer[LENGTH_PATH];
			if (OpenFileDialog(buffer))
			{
				HWND edit = m_areasOfFiles[i].GetEdit();
				SetWindowTextW(edit, buffer);
				if (!loadFile(i, buffer))
				{
					WCHAR message[LENGTH_PATH + MAX_SIZE_STRING];
					message[0] = 0;
					wcscat_s(message, L"Не удалось открыть файл ");
					wcscat_s(message, buffer);
					MessageBoxW(hWnd, message, L"", 0);
					return;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
		}
	}
}

bool DisplayArea::OpenFileDialog(LPWSTR fileName)
{
	OPENFILENAMEW ofn;
	ZeroMemory(fileName, LENGTH_PATH);
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd; 
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = LENGTH_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileNameW(&ofn);
}


void DisplayArea::Scroll()
{
	m_scrollInc = max(
		-scrollPos,
		min(m_scrollInc, m_countRows - m_minCountOfVisibleRows - scrollPos)
	);
	if (m_scrollInc != 0)
	{
		scrollPos += m_scrollInc;
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
