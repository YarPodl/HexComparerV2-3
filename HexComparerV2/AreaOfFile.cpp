#include "stdafx.h"
#include "AreaOfFile.h"



BOOL AreaOfFile::Initialize(
			INT number, HWND hWnd, HINSTANCE hInst, 
			HFONT hFont, 
			FileCommander * fileCommander, 
			DataOfScroll * dateOfScroll)
{
	// ����������� ��������
	m_NumberOfArea		= number;
	m_hWnd				= hWnd;
	m_hInst				= hInst;
	m_pFileCommander	= fileCommander;
	m_pDataOfScroll		= dateOfScroll;

	// �������� ��������� (���� ������������)
	CloseHandles();

	// ���� ����� �������� �����
	m_hEdit = CreateWindowW(
		L"edit",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_RectMenu.left, m_RectMenu.top,
		m_RectMenu.right - WIDTH_FILES_BUTTONS, HEIGHT_FILES_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// �������� ����������
	if (!m_hEdit)
		return FALSE;

	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);

	// ������ �������� �����
	m_hButton = CreateWindowW(
		L"button",
		L">>",
		WS_CHILD | WS_VISIBLE,
		m_RectMenu.left + m_RectMenu.right - WIDTH_FILES_BUTTONS, m_RectMenu.top,
		WIDTH_FILES_BUTTONS, HEIGHT_FILES_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// �������� ����������
	if (!m_hButton)
		return FALSE;

	// �������� ScrollBar
	m_hScrollBar = CreateWindowW(
		L"scrollbar",
		NULL,
		WS_CHILD | WS_VISIBLE | SBS_VERT,
		m_RectMenu.left + m_RectMenu.right - WIDTH_FILES_BUTTONS, 
		m_RectMenu.top,
		WIDTH_FILES_BUTTONS, 
		HEIGHT_FILES_BUTTONS,
		hWnd, NULL, hInst, NULL);

	// �������� ����������
	if (!m_hScrollBar)
		return FALSE;

	UpdateNumberOfRow();

	return TRUE;
}


void AreaOfFile::CloseHandles()
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
	// ��������� �����
	PaintBorder(hdc);

	// �������� �������� �� ����
	if (!m_pFileCommander->IsLoadedFile(m_NumberOfArea))
		return;
	
	INT		FirstPaintingRow;	// ������ �������� ������ (������ �� ������ �������)
	INT		LastPaintingRow;	// ��������� �������� ������ (������ �� ������ �������)

	// ���������� �������� ����� ����� �� �������� �������
	FirstPaintingRow = (ps.rcPaint.top - m_RectData.top) / m_HeightChar;
	LastPaintingRow  = (ps.rcPaint.bottom - m_RectData.top) / m_HeightChar + 1;

	// ����������� �� ����� �� ������� ���������
	if (FirstPaintingRow < 0)
	{
		FirstPaintingRow = 0;
	}
	if (LastPaintingRow > m_CountOfVisibleRows)
	{
		LastPaintingRow = m_CountOfVisibleRows + 1;
	}

	// �����
	SelectObject(hdc, m_hFont);	

	// ����������� ������
	PaintDataOfFile(hdc, FirstPaintingRow, LastPaintingRow);
}

void AreaOfFile::SetSize(RECT client)
{
	// ������ ������� � �������
	m_RectData			= client;
	m_RectData.top		= client.top + HEIGHT_MENU;
	m_RectData.right	= client.right - WIDTH_SCROLLBAR;

	// ������ ����
	m_RectMenu			= client;
	m_RectMenu.bottom	= client.top + HEIGHT_MENU;

	// ��������� ������� � ������� ���� �����
	SetWindowPos(m_hEdit, HWND_BOTTOM,
		m_RectMenu.left, m_RectMenu.top,
		m_RectMenu.right - m_RectMenu.left - WIDTH_FILES_BUTTONS, HEIGHT_FILES_BUTTONS,
		0);

	// ��������� ������� � ������� ������
	SetWindowPos(m_hButton, HWND_BOTTOM,
		m_RectMenu.right - WIDTH_FILES_BUTTONS, m_RectMenu.top,
		WIDTH_FILES_BUTTONS, HEIGHT_FILES_BUTTONS,
		0);

	// ��������� ������� � ������� ScrollBar
	SetWindowPos(m_hScrollBar, HWND_BOTTOM,
		m_RectData.right, m_RectData.top,
		WIDTH_SCROLLBAR, m_RectData.bottom - m_RectData.top,
		0);

	// ��������� ������� ������ � ������������ � ��������
	UpdateFont();

	// ���������� ������ � ��� ScrollBar
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


void AreaOfFile::UpdateData()
{
	UpdateNumberOfRow();
	UpdateFont();
	UpdateScrollInfo();
}


void AreaOfFile::Scroll()
{
	// ��������� �����������
	ScrollWindowEx(m_hWnd, 0, -m_HeightChar * m_pDataOfScroll->ScrollInc, &m_RectData, &m_RectData, NULL, NULL, SW_INVALIDATE);

	// ������� �������
	SetScrollPos(m_hScrollBar, SB_CTL, m_pDataOfScroll->ScrollPos / m_pDataOfScroll->RatioOfScroll + 0.5, true);

	// �����������
	UpdateWindow(m_hWnd);
}



void AreaOfFile::�hangeEdit(HWND hEdit)
{
	if (m_hEdit == hEdit)
	{
		m_pFileCommander->CloseFile(m_NumberOfArea);
	}
}


void AreaOfFile::�lickButton(HWND hButton)
{
	if (m_hButton == hButton)
	{
		WCHAR buffer[LENGTH_PATH];
		if (OpenFileDialog(buffer))
		{
			// ����������� �������� ���������� �����
			SetWindowTextW(m_hEdit, buffer);

			OpenFile();
		}
	}
}


void AreaOfFile::OpenFile()
{
	WCHAR	buffer[LENGTH_PATH]		= { 0 };	// ����� ��� �������� ���� � �����

	// ��������� ���� �� edit
	GetWindowTextW(m_hEdit, buffer, LENGTH_PATH);

	// �������� �����
	if (!m_pFileCommander->LoadFile(m_NumberOfArea, buffer))
	{
		WCHAR	message[LENGTH_PATH + MAX_SIZE_STRING] = { 0 };	// ����� ��� �������� ��������� ������������

		// ������������ ���������
		wcscat_s(message, L"�� ������� ������� ���� ");
		wcscat_s(message, buffer);

		MessageBoxW(m_hWnd, message, L"", 0);
	}

	// �����������
	InvalidateRect(m_hWnd, NULL, TRUE);
	UpdateWindow(m_hWnd);
}


BOOL AreaOfFile::OpenFileDialog(LPWSTR fileName)
{
	OPENFILENAMEW	ofn;

	ZeroMemory(fileName, LENGTH_PATH);
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize		= sizeof(OPENFILENAME);						// ������ ���������
	ofn.hwndOwner		= m_hWnd;									// ���� ��������
	ofn.lpstrFile		= fileName;									// ��� �����
	ofn.nMaxFile		= LENGTH_PATH;								// ������������ ����� ����
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	// �����

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
	WCHAR			m_buffer[LENGTH_OF_BUFFER]	= { 0 };	// ����� ��� ������ ������

	// ���������� ������
	wsprintfW(m_buffer, m_Format, numberLineForView);

	TextOutW(
		hdc,
		m_RectData.left + m_WidthChar * INDENT1,						// �������� �� X
		m_RectData.top + INDENT_OF_TOP + numberLine * m_HeightChar,		// �������� �� Y
		m_buffer,														// ������ ������
		wsprintfW(m_buffer, m_Format, numberLineForView)				// ����� ������ ������. ���������� ������ ������
	);
}


void AreaOfFile::PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte)
{
	// �������� �� Y
	INT y = m_RectData.top + INDENT_OF_TOP + numberLine * m_HeightChar;

	// ����������� ������� �����
	TextOutA(
		hdc,
		m_IndentForLetters + m_WidthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// ����������� ����� ��� Hex ������
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
	SCROLLINFO scrollInfo;		// ���������� � �������

	scrollInfo.cbSize	= sizeof(scrollInfo);
	scrollInfo.nMin		= 0;
	scrollInfo.nMax		= m_pDataOfScroll->CountRows > m_CountOfVisibleRows ? m_pDataOfScroll->MaxScrollPos : 0;
	scrollInfo.nPage	= m_CountOfVisibleRows / m_pDataOfScroll->RatioOfScroll;	// �������� ����� ������� ������ �������
	scrollInfo.fMask	= SIF_RANGE | SIF_PAGE;

	// ���� ������ ���������� �� ������
	if (m_pDataOfScroll->CountRows > m_CountOfVisibleRows)
	{
		scrollInfo.nMax		= m_pDataOfScroll->MaxScrollPos;
		ShowScrollBar(m_hScrollBar, SB_CTL, TRUE);
	}
	else
	{
		scrollInfo.nMax		= 0;
		ShowScrollBar(m_hScrollBar, SB_CTL, FALSE);
	}

	SetScrollInfo(m_hScrollBar, SB_CTL, &scrollInfo, TRUE);
}



void AreaOfFile::PaintDataOfFile(HDC hdc, INT FirstPaintingRow, INT LastPaintingRow)
{
	// ����� �������� ����� �� ������ �����
	INT64 NumberOfByte = (FirstPaintingRow + m_pDataOfScroll->ScrollPos) * LENGTH_OF_BYTE_STRINGS;

	// �������� ���� ������
	COLORREF BaseTextColor = GetTextColor(hdc);

	StateOfByte	State;										// ��������� �����
	BYTE		Byte = 0;									// ��������
	CHAR		CharOfByte = 0;								// ���� ��� ������	
	WCHAR		StringOfByte[LENGTH_OF_BYTE] = { 0 };		// ���� ��� Hex ������

	// ���� �� �������
	for (INT /* ��� ����� INT !! */ NumberRow = FirstPaintingRow; NumberRow < LastPaintingRow; NumberRow++)
	{
		if (CheckEnd(hdc, NumberRow))
			break;

		// ����������� ������ ������
		PaintNumberLine(hdc, NumberRow, (NumberRow + m_pDataOfScroll->ScrollPos) * LENGTH_OF_BYTE_STRINGS); 

			// ���� �� ������ � ������
			for (DWORD NumbOfByteInRow = 0; NumbOfByteInRow < LENGTH_OF_BYTE_STRINGS; NumbOfByteInRow++)
			{
				// ��������� ����� � ��� ��������� 
				State = m_pFileCommander->GetByte(m_NumberOfArea, NumberOfByte, Byte);

				// � ����������� �� ��������� �����
				switch (State)
				{
					// ���������� �������, ���� ���� ��������
				case FileEnded:
					CharOfByte		=  ' ';
					StringOfByte[0] = L' ';
					StringOfByte[1] = L' ';
					break;

					// ���������� ���� ������� ������, ���� ����� �����
				case ByteEqual:
					SetViewOfByte(hdc, BaseTextColor, Byte, CharOfByte, StringOfByte);
					break;

					// ���������� ���� ���������� ������, ���� ����� �� �����
				case ByteNotEqual:
					SetViewOfByte(hdc, TEXT_COLOR_SELECT, Byte, CharOfByte, StringOfByte);
					break;

				default:
					break;
				}

				// ��������� �����
				PaintByte(hdc, NumberRow, NumbOfByteInRow, StringOfByte, CharOfByte);

				NumberOfByte++;
			}

		// ������� ��������� �����
		SetTextColor(hdc, BaseTextColor);
	}
}

void AreaOfFile::SetViewOfByte(HDC hdc, COLORREF color, BYTE byte, OUT CHAR &charOfByte, OUT WCHAR stringOfByte[LENGTH_OF_BYTE])
{
	// ���� �����
	SetTextColor(hdc, color);

	// ������ �����
	charOfByte = byte <= 31 ? '.' : byte;

	// ����������������� ��� �����
	ByteToHexString(byte, stringOfByte);
}

BOOL AreaOfFile::CheckEnd(HDC hdc, DWORD NumberRow)
{
	// ���� ��� ����� �����������
	if (NumberRow + m_pDataOfScroll->ScrollPos >= m_pDataOfScroll->CountRows)
	{
		// ������� ����� ��������� ������ �������� �����
		RECT Rect	= m_RectData;
		Rect.top	= m_RectData.top + m_HeightChar * NumberRow;

		// �� �������� �����
		Rect.left++;
		Rect.right--;

		// ������� �������
		FillRect(hdc, &Rect, BACKGROUND_WINDOW);

		return TRUE;
	}

	return FALSE;
}

void inline AreaOfFile::ByteToHexString(byte in, OUT WCHAR out[])
{
	out[0] = (in >> 4) + L'0';		// ������ ������� ����� ������������������ �����
	out[1] = (in & 0x0F) + L'0';	// ������ ������� ����� ������������������ �����
	out[2] = 0;						// ����� ������

	// ���� ����� ����� > 0x9, ����������� ������� �� A,B,C..F
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
	// ���������� �������� � ����� ������
	INT CountCharsInRow = INDENT1 + INDENT2 + INDENT3 + m_LengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2) + LENGTH_OF_BYTE_STRINGS + INDENT4;

	// ������ �������, �����������, ����� ����������� ��� �������
	INT WidthChar = (m_RectData.right - m_RectData.left) / CountCharsInRow;

	// ������ �������, ��������������� ������
	INT HeightChar = (INT)((double)WidthChar / FONT_SIZE_RELATION + 0.5);

	// ���� ������ ������ ������������
	if (HeightChar > MAX_FONT_SIZE_HEIGHT)
	{
		WidthChar = (INT)(MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION);
		HeightChar = MAX_FONT_SIZE_HEIGHT;
	}

	// �������� ������� ������
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// �������� ������ � ������������ ���������
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

	TEXTMETRICW		tm;		// �������� ������� ������ 

	// ��������� ������
	GetTextMetricsW(hdc, &tm);

	// ����������� ������
	m_WidthChar = tm.tmAveCharWidth;
	m_HeightChar = tm.tmHeight + tm.tmExternalLeading + INTERLACED_SPACE;

	ReleaseDC(m_hWnd, hdc);

	// ���������� ���������� ������� �����
	m_CountOfVisibleRows = (m_RectData.bottom - m_RectData.top) / m_HeightChar;
	
	// ���������� �������� ��� ������������ �������������
	m_IndentForBytes = m_RectData.left + m_WidthChar * (INDENT1 + INDENT2 + m_LengthOfNumberRow);
	m_IndentForLetters = m_RectData.left + m_WidthChar * (INDENT1 + INDENT2 + INDENT3 + m_LengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2));
}


void AreaOfFile::UpdateNumberOfRow()
{
	// ������������ ���������� ����
	INT64 CountBytes = m_pDataOfScroll->CountRows * LENGTH_OF_BYTE_STRINGS;

	// ���������� ����� ������ � �������
	m_LengthOfNumberRow = 1;
	while ((CountBytes /= 16) != 0)
	{
		m_LengthOfNumberRow++;
	}

	// ������������ ������ ������� ��� ����������� ������ ������
	wsprintfW(m_Format, L"%%0%dIX:", m_LengthOfNumberRow);

	// ���� ��������� ����� ������
	m_LengthOfNumberRow++;	
}

