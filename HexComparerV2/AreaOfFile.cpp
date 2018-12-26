#include "stdafx.h"
#include "AreaOfFile.h"



BOOL AreaOfFile::Initialize(INT number, HWND hWnd, HINSTANCE hInst, HFONT hFont, FileCommander * fileCommander)
{
	// Запоминание значений
	m_NumberOfArea = number;
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_pFileCommander = fileCommander;

	// Удаление элементов (если существовали)
	CloseHandle();

	// Поле ввода названия файла
	m_hEdit = CreateWindowW(
		L"edit",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_RectMenu.left, m_RectMenu.top,
		m_RectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Проверка успешности
	if (!m_hEdit)
	{
		return FALSE;
	}

	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);

	// Кнопка открытия файла
	m_hButton = CreateWindowW(
		L"button",
		L">>",
		WS_CHILD | WS_VISIBLE,
		m_RectMenu.left + m_RectMenu.right - WIDTH_BUTTONS, m_RectMenu.top,
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
		m_RectMenu.left + m_RectMenu.right - WIDTH_BUTTONS, 
		m_RectMenu.top,
		WIDTH_BUTTONS, 
		HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// Проверка успешности
	if (!m_hScrollBar)
	{
		return FALSE;
	}

	UpdateNumberOfRow();

	return TRUE;
}


void AreaOfFile::CloseHandle()
{
	if (m_hEdit)
	{
		DestroyWindow(m_hEdit);
		m_hEdit = NULL;
	}

	if (m_hButton)
	{
		DestroyWindow(m_hButton);
		m_hButton = NULL;
	}

	if (m_hScrollBar)
	{
		DestroyWindow(m_hScrollBar);
		m_hScrollBar = NULL;
	}
}


void AreaOfFile::PaintArea(HDC hdc, PAINTSTRUCT & ps)
{
	// Рисование рамки
	PaintBorder(hdc);

	// Проверка загружен ли файл
	if (!m_pFileCommander->IsLoadedFile(m_NumberOfArea))
		return;

	
	INT		FirstPaintingRow;	// Первая рисуемая строка (считая от первой видимой)
	INT		LastPaintingRow;	// Последняя рисуемая строка (считая от первой видимой)

	// Вычисление рисуемых строк через не валидную область
	FirstPaintingRow = (ps.rcPaint.top - m_RectData.top) / m_HeightChar;
	LastPaintingRow  = (ps.rcPaint.bottom - m_RectData.top) / m_HeightChar + 1;

	// Ограничение на выход за пределы видимости
	if (FirstPaintingRow < 0)
	{
		FirstPaintingRow = 0;
	}
	if (LastPaintingRow > m_CountOfVisibleRows)
	{
		LastPaintingRow = m_CountOfVisibleRows + 1;
	}

	// Шрифт
	SelectObject(hdc, m_hFont);	

	// Номер текущего байта от начала файла
	INT64 NumberOfByte = (FirstPaintingRow + m_ScrollPos) * LENGTH_OF_BYTE_STRINGS;

	// Исходный цвет текста
	COLORREF BaseTextColor = GetTextColor(hdc);

	
	StateOfByte	State;										// Состояние байта
	BYTE		Byte							= 0;		// Значение
	CHAR		CharOfByte						= 0;		// Байт как символ	
	WCHAR		StringOfByte[LENGTH_OF_BYTE]	= { 0 };	// Байт как Hex строка

	// Цикл по строкам
	for (DWORD NumberRow = FirstPaintingRow; NumberRow < LastPaintingRow; NumberRow++)
	{		
		// Если все файлы закончились
		if (NumberRow >= m_CountRows)
		{
			// Область после окончания самого длинного файла
			RECT Rect = m_RectData;
			Rect.top = m_RectData.top + m_HeightChar * NumberRow;

			// Заливка области
			FillRect(hdc, &Rect, BACKGROUND_WINDOW);

			break;
		}

		// Отображение номера строки
		PaintNumberLine(hdc, NumberRow, (NumberRow + m_ScrollPos) * LENGTH_OF_BYTE_STRINGS);
		
		// Цикл по байтам в строке
		for (DWORD NumbOfByteInRow = 0; NumbOfByteInRow < LENGTH_OF_BYTE_STRINGS; NumbOfByteInRow++)
		{
			// Получение байта и его состояния 
			State = m_pFileCommander->GetByte(m_NumberOfArea, NumberOfByte, Byte);
			
			// В зависимости от состояния байта
			switch (State)
			{
			// Отобразить пробелы, если файл кончился
			case FileEnded:
				CharOfByte = ' ';
				StringOfByte[0] = L' ';
				StringOfByte[1] = L' ';
				break;

			// Отобразить байт базовым цветом, если байты равны
			case ByteEqual:
				SetTextColor(hdc, BaseTextColor);
				CharOfByte = Byte <= 31 ? '.' : Byte;
				ByteToHexString(Byte, StringOfByte);
				break;

			// Отобразить байт выделенным цветом, если байты не равны
			case ByteNotEqual:

				SetTextColor(hdc, TEXT_COLOR_SELECT);
				CharOfByte = Byte <= 31 ? '.' : Byte;
				ByteToHexString(Byte, StringOfByte);
				break;

			default:
				break;
			}

			// Отрисовка байта
			PaintByte(hdc, NumberRow, NumbOfByteInRow, StringOfByte, CharOfByte);

			NumberOfByte++;
			
		}

		// Возврат исходного цвета
		SetTextColor(hdc, BaseTextColor);
		
	}

}

void AreaOfFile::setSize(RECT client)
{
	// Размер области с данными
	m_RectData = client;
	m_RectData.top = client.top + HEIGHT_MENU;
	m_RectData.right = client.right - WIDTH_SCROLLBAR;

	// Размер меню
	m_RectMenu = client;
	m_RectMenu.bottom = client.top + HEIGHT_MENU;

	// Установка размера и позиции поля ввода
	SetWindowPos(m_hEdit, HWND_BOTTOM,
		m_RectMenu.left, m_RectMenu.top,
		m_RectMenu.right - m_RectMenu.left - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции кнопки
	SetWindowPos(m_hButton, HWND_BOTTOM,
		m_RectMenu.right - WIDTH_BUTTONS, m_RectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// Установка размера и позиции ScrollBar
	SetWindowPos(m_hScrollBar, HWND_BOTTOM,
		m_RectData.right, m_RectData.top,
		WIDTH_SCROLLBAR, m_RectData.bottom - m_RectData.top,
		0);

	// Изменение размера шрифта в соответствии с размером
	UpdateFont();

	// Обновление данных в для ScrollBar
	UpdateScrollInfo();
}


HWND AreaOfFile::GetButton()
{
	return m_hButton;
}


HWND AreaOfFile::GetEdit()
{
	return m_hEdit;
}


HWND AreaOfFile::GetScrollBar()
{
	return m_hScrollBar;
}


INT AreaOfFile::GetCountOfVisibleRows()
{
	return m_CountOfVisibleRows;
}


void AreaOfFile::SetData(INT64 countRows, double ratioOfScroll, INT maxScrollPos)
{
	m_RatioOfScroll		= ratioOfScroll;
	m_MaxScrollPos		= maxScrollPos;
	m_CountRows			= countRows;

	UpdateNumberOfRow();
	UpdateFont();
	UpdateScrollInfo();
}


void AreaOfFile::Scroll(INT64 scrollInc)
{
	// Новая позиция скролла
	m_ScrollPos += scrollInc;

	// Прокрутка изображения
	ScrollWindowEx(m_hWnd, 0, -m_HeightChar * scrollInc, &m_RectData, &m_RectData, NULL, NULL, SW_INVALIDATE);

	// Позиция бегунка
	SetScrollPos(m_hScrollBar, SB_CTL, m_ScrollPos / m_RatioOfScroll + 0.5, true);

	// Перерисовка
	UpdateWindow(m_hWnd);
}



void AreaOfFile::СhangeEdit(HWND hEdit)
{
	if (m_hEdit == hEdit)
	{
		m_pFileCommander->CloseFile(m_NumberOfArea);
	}
}


void AreaOfFile::СlickButton(HWND hButton)
{
	if (m_hButton == hButton)
	{
		WCHAR buffer[LENGTH_PATH];
		if (OpenFileDialog(buffer))
		{
			SetWindowTextW(m_hEdit, buffer);

			OpenFile();
		}
	}
}


void AreaOfFile::OpenFile()
{
	WCHAR	buffer[LENGTH_PATH]		= { 0 };	// Буфер для хранения пути к файлу

	// Получение пути из edit
	GetWindowTextW(m_hEdit, buffer, LENGTH_PATH);

	// Загрузка файла
	if (!m_pFileCommander->LoadFile(m_NumberOfArea, buffer))
	{
		WCHAR	message[LENGTH_PATH + MAX_SIZE_STRING] = { 0 };	// Буфер для хранения сообщения пользователю

		// Формирование сообщения
		wcscat_s(message, L"Не удалось открыть файл ");
		wcscat_s(message, buffer);

		MessageBoxW(m_hWnd, message, L"", 0);
	}

	// Перерисовка
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
}


BOOL AreaOfFile::OpenFileDialog(LPWSTR fileName)
{
	OPENFILENAMEW	ofn;

	ZeroMemory(fileName, LENGTH_PATH);
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize		= sizeof(OPENFILENAME);						// Размер структуры
	ofn.hwndOwner		= m_hWnd;									// Окно родитель
	ofn.lpstrFile		= fileName;									// Имя файла
	ofn.nMaxFile		= LENGTH_PATH;								// Максимальная длина пути
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	// Флаги

	return GetOpenFileNameW(&ofn);
}

void AreaOfFile::PaintBorder(HDC hdc)
{
	MoveToEx(hdc, m_RectData.left, m_RectMenu.top, NULL);

	LineTo(hdc, m_RectData.right, m_RectMenu.top);
	LineTo(hdc, m_RectData.right, m_RectData.bottom);
	LineTo(hdc, m_RectData.left, m_RectData.bottom);
	LineTo(hdc, m_RectData.left, m_RectMenu.top);
}


void AreaOfFile::PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView)
{
	WCHAR			m_buffer[LENGTH_OF_BUFFER]	= { 0 };	// Буфер для номера строки

	// Заполнение буфера
	wsprintfW(m_buffer, m_Format, numberLineForView);

	TextOutW(
		hdc,
		m_RectData.left + m_WidthChar * INDENT1,						// Смещение по X
		m_RectData.top + INDENT_OF_TOP + numberLine * m_HeightChar,		// Смещение по Y
		m_buffer,														// Строка номера
		wsprintfW(m_buffer, m_Format, numberLineForView)				// Длина строки номера. Вычисление строки номера
	);
}


void AreaOfFile::PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte)
{
	// Смещение по Y
	INT y = m_RectData.top + INDENT_OF_TOP + numberLine * m_HeightChar;

	// Отображение символа байта
	TextOutA(
		hdc,
		m_IndentForLetters + m_WidthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// Отображение байта как Hex строки
	TextOutW(
		hdc,
		m_IndentForBytes + m_WidthChar * (numberByte * (CharsForByte + INDENT_BETWEEN_BYTES2)),
		y,
		stringOfByte,
		CharsForByte
	);
}


void AreaOfFile::UpdateScrollInfo()
{
	SCROLLINFO scrollInfo;		// Информация о скролле

	scrollInfo.cbSize	= sizeof(scrollInfo);
	scrollInfo.nMin		= 0;
	scrollInfo.nMax		= m_CountRows > m_CountOfVisibleRows ? m_MaxScrollPos : 0;
	scrollInfo.nPage	= m_CountOfVisibleRows / m_RatioOfScroll;				// Возможно стоит сделать равным единице
	scrollInfo.fMask	= SIF_RANGE | SIF_PAGE;

	// Если текущая позиция скролла вышла за пределы, вернуть ее в начало
	if (m_ScrollPos > m_CountRows - m_CountOfVisibleRows)
	{
		m_ScrollPos			= 0;
		scrollInfo.nPos		= 0;
		scrollInfo.fMask	|= SIF_POS;
	}

	// Если строки помещаются на экране
	if (m_CountRows > m_CountOfVisibleRows)
	{
		scrollInfo.nMax		= m_MaxScrollPos;
		ShowScrollBar(m_hScrollBar, SB_CTL, TRUE);
	}
	else
	{
		scrollInfo.nMax		= 0;
		ShowScrollBar(m_hScrollBar, SB_CTL, FALSE);
	}

	SetScrollInfo(m_hScrollBar, SB_CTL, &scrollInfo, TRUE);
}



void inline AreaOfFile::ByteToHexString(byte in, OUT WCHAR out[])
{
	out[0] = (in >> 4) + L'0';		// Символ старшей части шестнадцатеричного числа
	out[1] = (in & 0x0F) + L'0';	// Символ младшей части шестнадцатеричного числа
	out[2] = 0;						// Конец строки

	// Если часть числа > 0x9, исправление символа на A,B,C..F
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
	// Количество символов в одной строке
	INT CountCharsInRow = INDENT1 + INDENT2 + INDENT3 + m_LengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2) + LENGTH_OF_BYTE_STRINGS + INDENT4;

	// Ширина символа, необходимая, чтобы поместились все символы
	INT WidthChar = (m_RectData.right - m_RectData.left) / CountCharsInRow;

	// Высота символа, соответствующая ширине
	INT HeightChar = (INT)((double)WidthChar / FONT_SIZE_RELATION + 0.5);

	// Если высота больше максимальной
	if (HeightChar > MAX_FONT_SIZE_HEIGHT)
	{
		WidthChar = (INT)(MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION);
		HeightChar = MAX_FONT_SIZE_HEIGHT;
	}

	// Удаление старого шрифта
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// Создание нового с вычисленными размерами
	m_hFont = CreateFontW(
		HeightChar,				
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

	HDC hdc = GetDC(m_hWnd);

	SelectObject(hdc, m_hFont);

	TEXTMETRICW		tm;		// Содержит метрики шрифта 

	// Получение метрик
	GetTextMetricsW(hdc, &tm);

	// Запоминание метрик
	m_WidthChar = tm.tmAveCharWidth;
	m_HeightChar = tm.tmHeight + tm.tmExternalLeading + INTERLACED_SPACE;

	ReleaseDC(m_hWnd, hdc);

	// Вычисление количества видимых строк
	m_CountOfVisibleRows = (m_RectData.bottom - m_RectData.top) / m_HeightChar;
	
	// Вычисление значений для последующего использования
	m_IndentForBytes = m_RectData.left + m_WidthChar * (INDENT1 + INDENT2 + m_LengthOfNumberRow);
	m_IndentForLetters = m_RectData.left + m_WidthChar * (INDENT1 + INDENT2 + INDENT3 + m_LengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2));
}


void AreaOfFile::UpdateNumberOfRow()
{
	// Максимальное количество байт
	INT64 CountBytes = m_CountRows * LENGTH_OF_BYTE_STRINGS;

	// Вычисление длины строки с номером
	m_LengthOfNumberRow = 1;
	while ((CountBytes /= 16) != 0)
	{
		m_LengthOfNumberRow++;
	}

	// Формирование строки формата для отображение номера строки
	wsprintfW(m_Format, L"%%0%dIX:", m_LengthOfNumberRow);

	// Учет двоеточия после номера
	m_LengthOfNumberRow++;	
}

