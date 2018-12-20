#include "stdafx.h"
#include "DisplayArea.h"

#define CALL_FOR_ALL_AREA(func) for (int i = 0; i < COUNT_OF_FILES; i++) { m_areasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	this->hWnd = hWnd;
	this->hInst = hInst;

	////////////////////////////

	HDC hdc = GetDC(hWnd);
	hFont = CreateFontW(FONT_SIZE_HEIGHT, 
				FONT_SIZE_WIDTH, 
				0, 0, 
				FW_NORMAL, 
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,		
				DEFAULT_QUALITY, 
				FIXED_PITCH | FF_MODERN, 
				FONT_FAMILY);

	SelectObject(hdc, hFont);

	
	ReleaseDC(hWnd, hdc);

	////////////////////////////////
}


void DisplayArea::SetSizeAreaOfFile()
{
	// Координаты одной области файла
	RECT ClientRectFileArea = { 0 };

	// Цикл прохода по областям
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		ClientRectFileArea.top		= 0;
		ClientRectFileArea.left		= widthClient / COUNT_OF_FILES * i;	
		ClientRectFileArea.right	= widthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= heightClient;
		
		m_areasOfFiles[i].setSize(ClientRectFileArea);
	}
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	heightClient = HIWORD(lParam);
	widthClient = LOWORD(lParam);

	SetSizeAreaOfFile();
	Scroll();

	// Минимальное количество видимых строк 
	m_minCountOfVisibleRows = m_areasOfFiles[0].getCountOfVisibleRows();

	// Цикл прохода по областям и поиска минимального значения для областей
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Scroll(m_scrollInc);
		if (m_minCountOfVisibleRows > m_areasOfFiles[i].getCountOfVisibleRows())
		{
			m_minCountOfVisibleRows = m_areasOfFiles[i].getCountOfVisibleRows();
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
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Paint(hdc, ps);
	}
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
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].getScrollBar() == (HWND)lParam)
		{
			SCROLLINFO scrollInfo;
			scrollInfo.fMask = SIF_TRACKPOS;
			GetScrollInfo((HWND)lParam, SB_CTL, &scrollInfo);
			//scrollPos = ratioOfScroll * scrollInfo.nTrackPos + 0.5;
			if (scrollInfo.nTrackPos + m_areasOfFiles[i].getCountOfVisibleRows() >= m_maxScrollPos)
			{
				scrollEnd();
			}
			else
			{
				m_scrollInc = ratioOfScroll * scrollInfo.nTrackPos - scrollPos;
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

bool DisplayArea::loadFile(int indexFile, LPCWSTR fileName)
{
	if (!m_fileCommander.LoadFile(indexFile, fileName))
	{
		return FALSE;
	}
	countOfByte = m_fileCommander.getMaxSize();
	m_countRows = countOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_maxScrollPos = m_countRows > MAXINT ? MAXINT : m_countRows;
	ratioOfScroll = m_countRows > MAXINT ? (double)m_countRows / m_maxScrollPos : 1;

	scrollPos = scrollPos > m_countRows ? m_maxScrollPos : scrollPos;


	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].setDateOfScroll(m_countRows, ratioOfScroll, m_maxScrollPos);
	}

	return TRUE;
}


int DisplayArea::getCountOfVisibleRows(LPARAM lParam)
{
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].getScrollBar() == (HWND)lParam)
		{
			return m_areasOfFiles[i].getCountOfVisibleRows();
		}
	}

	return 0;
}

void DisplayArea::СhangeEdit(LPARAM lParam)
{
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].getEdit() == (HWND)lParam)
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
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		if (m_areasOfFiles[i].getButton() == (HWND)lParam)
		{
			WCHAR buffer[LENGTH_PATH];
			if (OpenFileDialog(buffer))
			{
				HWND edit = m_areasOfFiles[i].getEdit();
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
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		m_areasOfFiles[i].Scroll(m_scrollInc);
	}
	m_scrollInc = max(
		-scrollPos,
		min(m_scrollInc, m_countRows - m_minCountOfVisibleRows - scrollPos)
	);
	if (m_scrollInc != 0)
	{
		scrollPos += m_scrollInc;
		for (int i = 0; i < COUNT_OF_FILES; i++)
		{
			m_areasOfFiles[i].Scroll(m_scrollInc);
		}
	}
}

DisplayArea::~DisplayArea()
{
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		m_fileCommander.CloseFile(i);
	}
}
