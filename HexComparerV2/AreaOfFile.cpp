#include "AreaOfFile.h"



AreaOfFile::AreaOfFile()
{
}


AreaOfFile::~AreaOfFile()
{
}

BOOL AreaOfFile::Initialize(int number, HWND hWnd, HINSTANCE hInst, FileCommander * fileCommander)
{
	m_numberOfArea = number;
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_fileCommander = fileCommander;

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
	if (!m_fileCommander->isLoadedFile(m_numberOfArea))
	{
		return;
	}

	int		firstPaintingRow;	// Первая рисуемая строка (считая от первой видимой)
	int		lastPaintingRow;	// Последняя рисуемая строка (считая от первой видимой)

	// Вычисление рисуемых строк через не валидную область
	firstPaintingRow = (ps.rcPaint.top - m_rectData.top) / m_heightChar;
	lastPaintingRow  = (ps.rcPaint.bottom - m_rectData.top) / m_heightChar + 1;

	// Ограничение на выход за пределы видимости
	if (firstPaintingRow < 0)
	{
		firstPaintingRow = 0;
	}
	if (lastPaintingRow >= m_countOfVisibleRows)
	{
		lastPaintingRow = m_countOfVisibleRows;
	}

	SelectObject(hdc, m_hFont);	// Шрифт

	// Номер текущего байта от начала файла
	INT64 numberOfByte = firstPaintingRow * LENGTH_OF_BYTE_STRINGS;

	// Исходный цвет текста
	COLORREF m_baseTextColor = GetTextColor(hdc);

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
		
		// Отображение номера строки
		PaintNumberLine(hdc, NumberRow, NumberRow + m_scrollPos);
		
		// Цикл по байтам в строке
		for (int NumbOfByteInRow = 0; NumbOfByteInRow < LENGTH_OF_BYTE_STRINGS; NumbOfByteInRow++)
		{
			State = m_fileCommander->getByte(m_numberOfArea, numberOfByte, Byte);
			
			switch (State)
			{
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

		// Возврат исходного цвета
		SetTextColor(hdc, m_baseTextColor);
		
	}

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
		m_rectMenu.right - m_rectMenu.left - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции кнопки
	SetWindowPos(m_hButton, HWND_BOTTOM,
		m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции ScrollBar
	SetWindowPos(m_hScrollBar, HWND_BOTTOM,
		m_rectData.right, m_rectData.top,
		WIDTH_SCROLLBAR, m_rectData.bottom - m_rectData.top,
		0);

	UpdateNumberOfRow();

	UpdateFont();

	// Обновление данных в для ScrollBar
	UpdateScrollInfo();
}

/*
void AreaOfFile::setFont(HFONT hFont)
{
	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);
}
*/


HWND AreaOfFile::getButton()
{
	return m_hButton;
}

HWND AreaOfFile::getEdit()
{
	return m_hEdit;
}

HWND AreaOfFile::getScrollBar()
{
	return m_hScrollBar;
}


int AreaOfFile::getCountOfVisibleRows()
{
	return m_countOfVisibleRows;
}

void AreaOfFile::setDateOfScroll(INT64 countRows, double ratioOfScroll, int maxScrollPos)
{
	m_ratioOfScroll = ratioOfScroll;
	m_maxScrollPos = maxScrollPos;
	m_countRows = countRows;

	UpdateNumberOfRow();
}

void AreaOfFile::Scroll(INT64 scrollInc)
{
	m_scrollPos += scrollInc;
	ScrollWindowEx(m_hWnd, 0, -m_heightChar * scrollInc, &m_rectData, &m_rectData, NULL, NULL, SW_INVALIDATE);
	SetScrollPos(m_hWnd, SB_VERT, m_scrollPos / m_ratioOfScroll + 0.5, true);
	UpdateWindow(m_hWnd);
}



void AreaOfFile::PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView)
{
	WCHAR			m_buffer[LENGTH_OF_BUFFER] = { 0 };
	TextOutW(
		hdc,
		m_rectData.left + m_widthChar * INDENT1,						// Смещение по X
		m_rectData.top + INDENT_OF_TOP + numberLine * m_heightChar,	// Смещение по Y
		m_buffer,													// Строка номера
		wsprintfW(m_buffer, m_format, numberLineForView)				// Длина строки номера. Вычисление строки номера
	);
}


void AreaOfFile::PaintByte(HDC hdc, int numberLine, int numberByte, WCHAR stringOfByte[], char charOfByte)
{
	// Смещение по Y
	int y = m_rectData.top + INDENT_OF_TOP + numberLine * m_heightChar;

	// Отображение символа байта
	TextOutA(
		hdc,
		m_indentForLetters + m_widthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// Отображение байта как Hex строки
	TextOutW(
		hdc,
		m_indentForBytes + m_widthChar * (numberByte * (CharsForByte + INDENT_BETWEEN_BYTES2)),
		y,
		stringOfByte,
		CharsForByte
	);
}


void AreaOfFile::UpdateScrollInfo()
{
	m_countOfVisibleRows = (m_rectData.bottom - m_rectData.top) / m_heightChar;

	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.nMin = 0;
	if (m_countRows > m_countOfVisibleRows)
	{
		scrollInfo.nMax = m_maxScrollPos;
		ShowScrollBar(m_hScrollBar, SB_CTL, TRUE);
	}
	else
	{
		scrollInfo.nMax = 0;
		ShowScrollBar(m_hScrollBar, SB_CTL, FALSE);
	}
	scrollInfo.nMax = m_countRows > m_countOfVisibleRows ? m_maxScrollPos : 0;
	scrollInfo.nPage = m_countOfVisibleRows;
	scrollInfo.fMask = SIF_RANGE | SIF_PAGE;

	SetScrollInfo(m_hScrollBar, SB_CTL, &scrollInfo, TRUE);
}



void AreaOfFile::ByteToHexString(byte in, WCHAR out[])
{
	out[0] = (in >> 4) + L'0';
	out[1] = (in & 0x0F) + L'0';
	out[2] = 0;
	if (out[0] > L'9')
	{
		out[0] = (out[0] - L'9' + L'A' - 1);
	}
	if (out[1] > L'9')
	{
		out[1] = (out[1] - L'9' + L'A' - 1);
	}
}



void AreaOfFile::UpdateFont()
{

	int CountCharsInRow = INDENT1 + INDENT2 + INDENT3 + m_lengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2) + LENGTH_OF_BYTE_STRINGS + INDENT4;

	int WidthChar = (m_rectData.right - m_rectData.left) / CountCharsInRow;


	HDC hdc = GetDC(m_hWnd);
	if (!m_hFont)
	{
		DeleteObject(m_hFont);
	}
	m_hFont = CreateFontW(WidthChar / FONT_SIZE_RELATION + 0.5,
		WidthChar,
		0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN,
		FONT_FAMILY);

	SelectObject(hdc, m_hFont);

	TEXTMETRICW tm;

	GetTextMetricsW(hdc, &tm);
	m_widthChar = tm.tmAveCharWidth;
	m_heightChar = tm.tmHeight + tm.tmExternalLeading + INTERLACED_SPACE;

	ReleaseDC(m_hWnd, hdc);

}

void AreaOfFile::UpdateNumberOfRow()
{
	m_lengthOfNumberRow = 1;
	while ((m_countRows /= 16) != 0)
	{
		m_lengthOfNumberRow++;
	}
	m_lengthOfNumberRow;
	wsprintfW(m_format, L"%%0%dlX:", m_lengthOfNumberRow);
	m_lengthOfNumberRow++;	// Двоеточие
	m_indentForBytes = m_rectData.left + m_widthChar * (INDENT1 + INDENT2 + m_lengthOfNumberRow);
	m_indentForLetters = m_rectData.left + m_widthChar * (INDENT1 + INDENT2 + INDENT3 + m_lengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2));
}

