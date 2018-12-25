#include "stdafx.h"
#include "DisplayArea.h"

// Вызов метода из параметра для каждого элемента массива m_AreasOfFiles
#define CALL_FOR_ALL_AREA(func) for (DWORD i = 0; i < COUNT_OF_FILES; i++) { m_AreasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	// Запоминание дескрипторов
	this->m_hWnd = hWnd;
	this->m_hInst = hInst;

	// Создание основного шрифта
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

	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// Инициализация областей
		m_AreasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_FileCommander);
	}
}


void DisplayArea::SetSizeAreaOfFile()
{
	RECT	ClientRectFileArea = { 0 };		// Координаты одной области файла
	INT		i;								// Переменная цикла

	// Цикл прохода по областям
	for (i = 0; i < COUNT_OF_FILES; i++)
	{
		// Координаты области
		ClientRectFileArea.top		= 0;
		ClientRectFileArea.left		= m_WidthClient / COUNT_OF_FILES * i;
		ClientRectFileArea.right	= m_WidthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_HeightClient;
		
		// Передача размера и данных о файлах
		m_AreasOfFiles[i].setSize(ClientRectFileArea);
		m_AreasOfFiles[i].SetData(m_CountRows, m_RatioOfScroll, m_MaxScrollPos);
	}
}


void DisplayArea::UpdateData()
{
	// Присвоить новое максимальное количество байт
	m_CountOfByte = m_FileCommander.GetMaxSize();

	// Перевычислить все параметры
	m_CountRows = m_CountOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_CountRows = m_CountOfByte == 0 ? 0 : m_CountRows;
	m_MaxScrollPos = m_CountRows > MAXINT ? MAXINT : m_CountRows;
	m_RatioOfScroll = m_CountRows > MAXINT ? (double)m_CountRows / m_MaxScrollPos : 1;
	m_ScrollPos = m_ScrollPos > m_CountRows ? m_MaxScrollPos : m_ScrollPos;

	// Передать новые параметры всем областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_AreasOfFiles[i].SetData(m_CountRows, m_RatioOfScroll, m_MaxScrollPos);
	}
		
	// Перерисовать все
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);

	UpdateMinCountOfVisibleRows();
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	m_HeightClient = HIWORD(lParam);
	m_WidthClient = LOWORD(lParam);

	// Передать новые размеры областям
	SetSizeAreaOfFile();

	// Установить корректный скролл
	m_ScrollInc = 0;
	Scroll();

	// Минимальное количество видимых строк 
	UpdateMinCountOfVisibleRows();
}


void DisplayArea::Paint(HDC hdc, PAINTSTRUCT &ps)
{
	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// Рисование каждой области
		m_AreasOfFiles[i].PaintArea(hdc, ps);
	}

	// Возврат базового шрифта
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
	SCROLLINFO	ScrollInfo;		// Информация о скролле

	// Получение информации о скролле
	ScrollInfo.cbSize = sizeof(ScrollInfo);
	ScrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo);

	// Если бегунок в конце полосы
	if (ScrollInfo.nTrackPos + m_MinCountOfVisibleRows + 1 >= m_MaxScrollPos)
	{
		ScrollEnd();
	}
	else
	{
		// Вычисление реального сдвига позиции скролла
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
	// Минимальное количество видимых строк 
	INT MinCountOfVisibleRows = MAXINT;

	// Цикл прохода по областям и поиска минимального значения для областей
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// Учитываются только области имеющие загруженный файл
		if ((m_FileCommander.IsLoadedFile(i)) && (MinCountOfVisibleRows > m_AreasOfFiles[i].GetCountOfVisibleRows()))
		{
			MinCountOfVisibleRows = m_AreasOfFiles[i].GetCountOfVisibleRows();
		}
	}

	// Если ни один файл не загружен
	if (MinCountOfVisibleRows == MAXINT)
	{
		MinCountOfVisibleRows = m_AreasOfFiles[0].GetCountOfVisibleRows();
	}

	m_MinCountOfVisibleRows = MinCountOfVisibleRows;
}


bool DisplayArea::OpenFileFromEdit()
{
	HWND FocusWindow = GetFocus();	// Окно, захватившее фокус

	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// Если фокус в поле редактирования
		if (m_AreasOfFiles[i].GetEdit() == FocusWindow)
		{
			// Открыть файл до данному полю
			m_AreasOfFiles[i].OpenFile();

			// Пересчитать параметры
			UpdateData();

			return TRUE;
		}
	}

	return FALSE;
}


void DisplayArea::СhangeEdit(HWND hEdit)
{
	CALL_FOR_ALL_AREA(СhangeEdit(hEdit));
	UpdateData();
}


void DisplayArea::СlickButton(HWND hButton)
{
	CALL_FOR_ALL_AREA(СlickButton(hButton));
	UpdateData();
}


void DisplayArea::Scroll()
{
	// Проверка на выход за границы
	m_ScrollInc = max(
		-m_ScrollPos,
		min(m_ScrollInc, m_CountRows - m_MinCountOfVisibleRows - m_ScrollPos)
	);

	// Если необходимо скроллить
	if (m_ScrollInc != 0)
	{
		// Изменить позицию
		m_ScrollPos += m_ScrollInc;

		// Выполнить скролл для каждой области
		CALL_FOR_ALL_AREA(Scroll(m_ScrollInc))
	}
}


DisplayArea::~DisplayArea()
{
	// Закрыть все файлы
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_FileCommander.CloseFile(i);
	}
}
