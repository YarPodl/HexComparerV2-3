#include "stdafx.h"
#include "DisplayArea.h"
#include <thread>

// Вызов метода из параметра для каждого элемента массива m_AreasOfFiles
#define CALL_FOR_ALL_AREAS(func) for (DWORD i = 0; i < COUNT_OF_FILES; i++) { m_AreasOfFiles[i].func; }


DisplayArea::DisplayArea(HWND hWnd, HINSTANCE hInst)
{
	// Запоминание дескрипторов
	m_hWnd	= hWnd;
	m_hInst	= hInst;

	// Создание основного шрифта
	HDC hdc = GetDC(hWnd);

	m_hFont = CreateFontW(MAX_FONT_SIZE_HEIGHT, 
				(int)(MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION - 1),
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
		WIDTH_BUTTONS,
		HEIGHT_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// Кнопка поиска предыдущего отличия
	m_hButtonPrev = CreateWindowW(
		L"button",
		L"Предыдущее отличие",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 2 + WIDTH_BUTTONS, 0,
		WIDTH_BUTTONS, 
		HEIGHT_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// Кнопка поиска количества отличий
	m_hButtonSearch = CreateWindowW(
		L"button",
		L"Сравнить",
		WS_CHILD | WS_VISIBLE,
		INDENT_BUTTON * 3 + WIDTH_BUTTONS * 2, 0,
		WIDTH_BUTTONS, 
		HEIGHT_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// Шрифт для кнопок
	SendMessageW(m_hButtonNext,		WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonPrev,		WM_SETFONT, (WPARAM)m_hFont, 1);
	SendMessageW(m_hButtonSearch,	WM_SETFONT, (WPARAM)m_hFont, 1);

	// Цикл прохода по областям
	for (DWORD i = 0; i < COUNT_OF_FILES; i++)
	{
		// Инициализация областей
		m_AreasOfFiles[i].Initialize(i, hWnd, hInst, m_hFont, &m_FileCommander, &m_DataOfScroll);
	}
}


void DisplayArea::SetSizeAreaOfFile()
{
	RECT	ClientRectFileArea = { 0 };		// Координаты одной области файла

	// Цикл прохода по областям
	for (DWORD i = 0; i < COUNT_OF_FILES; i++)
	{
		// Координаты области
		ClientRectFileArea.top		= HEIGHT_BUTTONS * 1.5;
		ClientRectFileArea.left		= m_WidthClient / COUNT_OF_FILES * i;
		ClientRectFileArea.right	= m_WidthClient / COUNT_OF_FILES * (i + 1);
		ClientRectFileArea.bottom	= m_HeightClient;
		
		// Передача размера и данных о файлах
		m_AreasOfFiles[i].SetSize(ClientRectFileArea);
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
	CALL_FOR_ALL_AREAS(UpdateData())

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

DWORD WINAPI DisplayArea::NextDifference(LPVOID lpParameter)
{
	DisplayArea * DisplayAreaObj = (DisplayArea *)lpParameter;

	return DisplayAreaObj->SearchDifference(FALSE);
}

DWORD WINAPI DisplayArea::PrevDifference(LPVOID lpParameter)
{
	DisplayArea * DisplayAreaObj = (DisplayArea *)lpParameter;

	return DisplayAreaObj->SearchDifference(TRUE);
}

DWORD DisplayArea::SearchDifference(BOOL reverseLookup)
{
	// Начался поиск, блокировка действий пользователя
	EnableWindow(m_hWnd, FALSE);
	m_Search = TRUE;

	PaintStringMessage((CHAR *)"Идет поиск, для отмены нажмите esc");

	// Номер байта, с которого начинается поиск
	INT64 BeginOfSearch	= reverseLookup 
		? m_DataOfScroll.ScrollPos * LENGTH_OF_BYTE_STRINGS - 1
		: (m_DataOfScroll.ScrollPos + 1) * LENGTH_OF_BYTE_STRINGS;

	INT64 Result = m_FileCommander.FindDifference(
		BeginOfSearch,
		reverseLookup ? -1 : 1,
		m_Search
	);

	memset(m_Message, 0, MAX_SIZE_STRING);

	// Если поиск прерван
	if (Result == SEARCH_CANCELED)
	{
		strcpy(m_Message, "Поиск прерван");
	}

	// Если ничего не найдено
	if (Result == DIFFERENCE_NOT_FOUND)
	{
		strcpy(m_Message, "Различий не найдено");
	}

	// Смещение для скролла к следующему различию
	m_DataOfScroll.ScrollInc = Result / LENGTH_OF_BYTE_STRINGS - m_DataOfScroll.ScrollPos;

	PaintStringMessage();

	// Разблокировка
	m_Search = FALSE;
	EnableWindow(m_hWnd, TRUE);

	Scroll();

	return 0;
}

DWORD WINAPI DisplayArea::CountDifference(LPVOID lpParameter)
{
	DisplayArea * DisplayAreaObj = (DisplayArea *)lpParameter;

	// Начался поиск, блокировка действий пользователя
	EnableWindow(DisplayAreaObj->m_hWnd, FALSE);
	DisplayAreaObj->m_Search = TRUE;

	DisplayAreaObj->PaintStringMessage((CHAR *)"Идет поиск, для отмены нажмите esc");

	INT64	Result	= 0;	// Результат поиска
	INT64	Count	= -1;	// Количество отличий

	while (Result >= 0)
	{
		Count++;
		Result = DisplayAreaObj->m_FileCommander.FindDifference(
			Result + 1,						// Поиск, начиная c места остановки
			1,								// Поиск идет вперед
			DisplayAreaObj->m_Search		// Остановка поиска
		);
	}

	// Если поиск прерван
	if (Result == SEARCH_CANCELED)
	{
		wsprintfA(DisplayAreaObj->m_Message, "Поиск прерван, найдено %d различий", Count);
	}

	// Если ничего не найдено
	if (Result == DIFFERENCE_NOT_FOUND)
	{
		wsprintfA(DisplayAreaObj->m_Message, "Найдено %d различий", Count);
	}

	DisplayAreaObj->PaintStringMessage();

	// Разблокировка
	DisplayAreaObj->m_Search = FALSE;
	EnableWindow(DisplayAreaObj->m_hWnd, TRUE);

	return 0;
}

void DisplayArea::PaintStringMessage(HDC hdc)
{
	CHAR	Message[MAX_SIZE_STRING];	// Буфер для вывода

	// Занесение сообщения в буфер и заполнение остатка пробелами
	memset(Message, ' ', MAX_SIZE_STRING);

	strncpy(Message, m_Message, strlen(m_Message));

	// Установка базового шрифта
	SelectObject(hdc, m_hFont);

	// Отображение байта как Hex строки
	TextOutA(
		hdc,
		INDENT_BUTTON * 4 + WIDTH_BUTTONS * 3, 0,
		Message,
		MAX_SIZE_STRING
	);
}

void DisplayArea::PaintStringMessage(CHAR * message)
{
	if (message)
	{
		memset(m_Message, 0, MAX_SIZE_STRING);
		strcpy(m_Message, message);
	}

	HDC hdc = GetDC(m_hWnd);

	PaintStringMessage(hdc);

	ReleaseDC(m_hWnd, hdc);
}


void DisplayArea::ChangeSize(LPARAM lParam)
{
	// Сохранение новых размеров клиентской области
	m_HeightClient	= HIWORD(lParam);
	m_WidthClient	= LOWORD(lParam);

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
	// Рисование каждой области
	CALL_FOR_ALL_AREAS(PaintArea(hdc, ps))

	PaintStringMessage(hdc);
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
	ScrollInfo.cbSize	= sizeof(ScrollInfo);
	ScrollInfo.fMask	= SIF_TRACKPOS;

	GetScrollInfo((HWND)lParam, SB_CTL, &ScrollInfo);

	// Вычисление реального сдвига позиции скролла
	m_DataOfScroll.ScrollInc = (INT64)
		(m_DataOfScroll.RatioOfScroll * ScrollInfo.nTrackPos + 0.5) - m_DataOfScroll.ScrollPos;

	Scroll();
}



INT DisplayArea::GetCountOfVisibleRows(LPARAM lParam)
{
	for (DWORD i = 0; i < COUNT_OF_FILES; i++)
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
	for (DWORD i = 0; i < COUNT_OF_FILES; i++)
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
	if (m_Search)
		return FALSE;

	HWND FocusWindow = GetFocus();	// Окно, захватившее фокус

	for (DWORD i = 0; i < COUNT_OF_FILES; i++)
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
	CALL_FOR_ALL_AREAS(СhangeEdit(hEdit));
	UpdateData();
}


void DisplayArea::СlickButton(HWND hButton)
{
	/*if (hButton == m_hButtonNext)
	{
		CreateThread(NULL, 0, NextDifference, this, 0, NULL);
	}
	else if (hButton == m_hButtonPrev)
	{
		CreateThread(NULL, 0, PrevDifference, this, 0, NULL);
	}
	else if (hButton == m_hButtonSearch)
	{
		CreateThread(NULL, 0, CountDifference, this, 0, NULL);
	}*/
	/*if (hButton == m_hButtonNext)
	{
		NextDifference(this);
	}
	else if (hButton == m_hButtonPrev)
	{
		PrevDifference(this);
	}
	else if (hButton == m_hButtonSearch)
	{
		CountDifference(this);
	}*/
	/*if (hButton == m_hButtonNext)
	{
		std::thread Thr(NextDifference, this);
	}
	else if (hButton == m_hButtonPrev)
	{
		std::thread Thr(PrevDifference, this);
	}
	else if (hButton == m_hButtonSearch)
	{
		std::thread Thr(CountDifference, this);
	}*/
	if (hButton == m_hButtonNext)
	{
		std::thread Thr(NextDifference, this);
		Thr.detach();
	}
	else if (hButton == m_hButtonPrev)
	{
		std::thread Thr(PrevDifference, this);
		Thr.detach();
	}
	else if (hButton == m_hButtonSearch)
	{
		std::thread Thr(CountDifference, this);
		Thr.detach();
	}
	else
	{
		CALL_FOR_ALL_AREAS(СlickButton(hButton));
		UpdateData();
	}
}

void DisplayArea::CancelSearch()
{
	m_Search = FALSE;
}


void DisplayArea::Scroll()
{
	// Проверка, идет ли сейчас поиск
	if (m_Search)
		return;

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
		CALL_FOR_ALL_AREAS(Scroll())
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
