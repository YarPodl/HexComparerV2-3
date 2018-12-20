#include "AreaOfFile.h"



AreaOfFile::AreaOfFile()
{
}


AreaOfFile::~AreaOfFile()
{
}

BOOL AreaOfFile::Initialize(HWND hWnd, HINSTANCE hInst)
{
	// Удаление элементов (если существовали)
	CloseHandle();

	// Поле ввода названия файла
	m_hEdit = CreateWindowW(
		L"edit",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_rectMenu.left, m_rectMenu.top,
		m_rectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Проверка успешности
	if (!m_hEdit)
	{
		return FALSE;
	}

	// Кнопка открытия файла
	m_hButton = CreateWindowW(
		L"button",
		L">>",
		WS_CHILD | WS_VISIBLE,
		m_rectMenu.left + m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Проверка успешности
	if (!m_hButton)
	{
		return FALSE;
	}

	// Создание ScrollBar
	m_hScrollBar = CreateWindowW(
		L"scrollbar",
		NULL,
		WS_CHILD | WS_VISIBLE | SBS_VERT,
		m_rectMenu.left + m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Проверка успешности
	if (!m_hScrollBar)
	{
		return FALSE;
	}

	return TRUE;
}

void AreaOfFile::CloseHandle()
{
	if (m_hEdit)
	{
		DestroyWindow(m_hEdit);
	}

	if (m_hButton)
	{
		DestroyWindow(m_hButton);
	}

	if (m_hScrollBar)
	{
		DestroyWindow(m_hScrollBar);
	}
}

void AreaOfFile::Paint(HDC hdc, PAINTSTRUCT & ps)
{
	int		firstPaintingRow;	// Первая рисуемая строка (считая от первой видимой)
	int		lastPaintingRow;	// Последняя рисуемая строка (считая от первой видимой)

	// Вычисление рисуемых строк через не валидную область
	firstPaintingRow = (ps.rcPaint.top - m_rectData.top) / heightChar;
	lastPaintingRow  = (ps.rcPaint.bottom - m_rectData.top) / heightChar + 1;

	// Ограничение на выход за пределы видимости
	if (firstPaintingRow < 0)
	{
		firstPaintingRow = 0;
	}
	if (lastPaintingRow >= m_countOfVisibleRows)
	{
		lastPaintingRow = m_countOfVisibleRows - 1;
	}

	SelectObject(hdc, m_hFont);	// Шрифт

	// Номер текущего байта от начала файла
	INT64 numberOfByte = firstPaintingRow * LENGTH_OF_BYTE_STRINGS;

	// Исходный цвет текста
	COLORREF m_baseTextColor = GetTextColor(hdc);

	// Отображение номеров строк
	for (int NumberRow = firstPaintingRow; NumberRow < lastPaintingRow; NumberRow++)
	{
		PaintNumberLine(hdc, NumberRow, NumberRow + m_scrollPos);
	}

	// Состояние байта
	StateOfByte	State;
	// Значение
	BYTE		Byte							= 0;
	// Байт как символ
	char		charOfByte						= 0;
	// Байт как Hex строка
	WCHAR		stringOfByte[LENGTH_OF_BYTE]	= { 0 };

	// Цикл по строкам
	for (int NumberRow = firstPaintingRow; NumberRow < lastPaintingRow; NumberRow++)
	{
		// Цикл по байтам в строке
		for (int NumbOfByteInRow = 0; NumbOfByteInRow < LENGTH_OF_BYTE_STRINGS; NumbOfByteInRow++)
		{
			State = m_fileCommander->getByte(m_numberOfArea, numberOfByte, Byte);

			switch (State)
			{
			case FileNotLoaded:
				return;

			case FileEnded:
				charOfByte = ' ';
				stringOfByte[0] = L'-';
				stringOfByte[1] = L'-';
				break;

			case ByteEqual:
				SetTextColor(hdc, m_baseTextColor);
				charOfByte = Byte <= 31 ? '.' : Byte;
				ByteToHexString(Byte, stringOfByte);
				break;

			case ByteNotEqual:
				SetTextColor(hdc, TEXT_COLOR_SELECT);
				charOfByte = Byte <= 31 ? '.' : Byte;
				ByteToHexString(Byte, stringOfByte);
				break;

			default:
				break;
			}
			// Отрисовка байта
			PaintByte(hdc, NumberRow, NumbOfByteInRow, stringOfByte, charOfByte);

			numberOfByte++;
		}
	}

	// Возврат исходного цвета
	SetTextColor(hdc, m_baseTextColor);
}

void AreaOfFile::setSize(RECT client)
{
	// Размер области с данными
	m_rectData = client;
	m_rectData.top = client.top + HEIGHT_MENU;
	m_rectData.right = client.right - WIDTH_SCROLLBAR;

	// Размер меню
	m_rectMenu = client;
	m_rectMenu.bottom = client.top + HEIGHT_MENU;

	// Установка размера и позиции поля ввода
	SetWindowPos(m_hEdit, HWND_BOTTOM,
		m_rectMenu.left, m_rectMenu.top,
		m_rectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции кнопки
	SetWindowPos(m_hButton, HWND_BOTTOM,
		m_rectMenu.left + m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции ScrollBar
	SetWindowPos(m_hScrollBar, HWND_BOTTOM,
		m_rectData.left + m_rectData.right - WIDTH_SCROLLBAR, m_rectData.top,
		WIDTH_SCROLLBAR, m_rectData.bottom - m_rectData.top,
		0);

	// Обновление данных в для ScrollBar
	UpdateScrollInfo();
}

void AreaOfFile::setFont(HFONT hFont)
{
	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);
}

void AreaOfFile::setFileCommander(FileCommander * fileCommander)
{
	m_fileCommander = fileCommander;
}

HWND AreaOfFile::getButton()
{
	return HWND();
}

HWND AreaOfFile::getEdit()
{
	return HWND();
}

HWND AreaOfFile::getScrollBar()
{
	return HWND();
}

int AreaOfFile::getCountOfVisibleRows()
{
	return m_countOfVisibleRows;
}

void AreaOfFile::setDateOfScroll(INT64 countRows, double ratioOfScroll, int maxScrollPos)
{
	m_ratioOfScroll = ratioOfScroll;
	m_maxScrollPos = maxScrollPos;
}

void AreaOfFile::Scroll(INT64 scrollInc)
{
	m_scrollPos += scrollInc;
	ScrollWindowEx(hWnd, 0, -heightChar * scrollInc, &m_rectData, &m_rectData, NULL, NULL, SW_INVALIDATE);
	SetScrollPos(hWnd, SB_VERT, m_scrollPos / m_ratioOfScroll + 0.5, true);
	UpdateWindow(hWnd);
}



void AreaOfFile::PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView)
{
	TextOutW(
		hdc,
		m_rectData.left + widthChar * INDENT1,						// Смещение по X
		m_rectData.top + INDENT_OF_TOP + numberLine * heightChar,	// Смещение по Y
		m_buffer,													// Строка номера
		wsprintfW(m_buffer, format, numberLineForView)				// Длина строки номера. Вычисление строки номера
	);
}


void AreaOfFile::PaintByte(HDC hdc, int numberLine, int numberByte, WCHAR stringOfByte[], char charOfByte)
{
	// Смещение по Y
	int y = m_rectData.top + INDENT_OF_TOP + numberLine * heightChar;

	// Отображение символа байта
	TextOutA(
		hdc,
		indentForLetters + widthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// Отображение байта как Hex строки
	TextOutW(
		hdc,
		indentForBytes + widthChar * (numberByte * (CharsForByte + INDENT_BETWEEN_BYTES2)),
		y,
		stringOfByte,
		CharsForByte
	);
}


void AreaOfFile::UpdateScrollInfo()
{
	m_countOfVisibleRows = (m_rectData.bottom - m_rectData.top) / heightChar;

	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.nMin = 0;
	scrollInfo.nMax = m_countRows > m_countOfVisibleRows ? m_maxScrollPos : 0;
	scrollInfo.nPage = m_countOfVisibleRows;
	scrollInfo.fMask = SIF_RANGE | SIF_PAGE;

	SetScrollInfo(m_hScrollBar, SB_CTL, &scrollInfo, TRUE);
}



void AreaOfFile::ByteToHexString(byte in, WCHAR out[])
{
	out[0] = (in >> 4) + L'0';
	out[1] = (in & 0x0F) + L'0';
	out[3] = 0;
	if (out[0] > L'9')
	{
		out[0] = (out[0] - L'9' + L'A' - 1);
	}
	if (out[1] > L'9')
	{
		out[1] = (out[1] - L'9' + L'A' - 1);
	}
}

