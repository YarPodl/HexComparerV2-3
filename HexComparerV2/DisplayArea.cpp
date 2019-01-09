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


	// Кнопка поиска следующего отличия
	m_hButtonNext = CreateWindowW(
		L"button",
		L"Следующее отличие",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Кнопка поиска предыдущего отличия
	m_hButtonPrev = CreateWindowW(
		L"button",
		L"Предыдущее отличие",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 2 + WIDTH_BUTTONS, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Кнопка поиска количества отличий
	m_hButtonSearch = CreateWindowW(
		L"button",
		L"Сравнить",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 3 + WIDTH_BUTTONS * 2, 0,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Шрифт для кнопок
	SendMessageW(m_hButtonNext, WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonPrev, WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonSearch, WM_SETFONT, (WPARAM)m_hFont, 1);

	// Цикл прохода по областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		// Инициализация областей
		m_AreasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_FileCommander, &m_DataOfScroll);
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
		ClientRectFileArea.top		= HEIGHT_BUTTONS * 1.5;
		ClientRectFileArea.left		= m_WidthClient / COUNT_OF_FILES * i;
		ClientRectFileArea.right	= m_WidthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_HeightClient;
		
		// Передача размера и данных о файлах
		m_AreasOfFiles[i].setSize(ClientRectFileArea);
		m_AreasOfFiles[i].UpdateData();
	}
}


void DisplayArea::UpdateData()
{
	// Присвоить новое максимальное количество байт
	m_CountOfByte = m_FileCommander.GetMaxSize();

	// Перевычислить все параметры
	m_DataOfScroll.CountRows		= m_CountOfByte / LENGTH_OF_BYTE_STRINGS + 1;
	m_DataOfScroll.CountRows		= m_CountOfByte == 0 ? 0 : m_DataOfScroll.CountRows;
	m_DataOfScroll.MaxScrollPos		= m_DataOfScroll.CountRows > MAXINT ? MAXINT : m_DataOfScroll.CountRows;
	m_DataOfScroll.RatioOfScroll	= m_DataOfScroll.CountRows > MAXINT
											? (double)m_DataOfScroll.CountRows / m_DataOfScroll.MaxScrollPos
											: 1;
	m_DataOfScroll.ScrollPos		= m_DataOfScroll.ScrollPos > m_DataOfScroll.CountRows
											? m_DataOfScroll.MaxScrollPos
											: m_DataOfScroll.ScrollPos;

	// Передать новые параметры всем областям
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		m_AreasOfFiles[i].UpdateData();
	}

	UpdateMinCountOfVisibleRows();


	// Если текущая позиция скролла вышла за пределы, вернуть ее в начало
	if (m_DataOfScroll.ScrollPos > m_DataOfScroll.CountRows - m_MinCountOfVisibleRows)
	{
		m_DataOfScroll.ScrollPos = 0;
	}

	// Перерисовать все
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	m_HeightClient = HIWORD(lParam);
	m_WidthClient = LOWORD(lParam);

	// Передать новые размеры областям
	SetSizeAreaOfFile();

	// Установить корректный скролл
	m_DataOfScroll.ScrollInc = 0;
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
	SCROLLINFO	ScrollInfo;		// Информация о скролле

	// Получение информации о скролле
	ScrollInfo.cbSize = sizeof(ScrollInfo);
	ScrollInfo.fMask = SIF_TRACKPOS;
	GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo);

	// Вычисление реального сдвига позиции скролла
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
	m_DataOfScroll.ScrollInc = max(
		-m_DataOfScroll.ScrollPos,
		min(m_DataOfScroll.ScrollInc, m_DataOfScroll.CountRows - m_MinCountOfVisibleRows - m_DataOfScroll.ScrollPos)
	);

	// Если необходимо скроллить
	if (m_DataOfScroll.ScrollInc != 0)
	{
		// Изменить позицию
		m_DataOfScroll.ScrollPos += m_DataOfScroll.ScrollInc;

		// Выполнить скролл для каждой области
		CALL_FOR_ALL_AREA(Scroll())
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
