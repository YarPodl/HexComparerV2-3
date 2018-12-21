#include "AreaOfFile.h"



AreaOfFile::AreaOfFile()
{
}


AreaOfFile::~AreaOfFile()
{
}

BOOL AreaOfFile::Initialize(INT number, HWND hWnd, HINSTANCE hInst, HFONT hFont, FileCommander * fileCommander)
{
	m_numberOfArea = number;
	m_hWnd = hWnd;
	m_hInst = hInst;
	m_fileCommander = fileCommander;

	// �������� ��������� (���� ������������)
	CloseHandle();

	// ���� ����� �������� �����
	m_hEdit = CreateWindowW(
		L"edit",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		m_rectMenu.left, m_rectMenu.top,
		m_rectMenu.right - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// �������� ����������
	if (!m_hEdit)
	{
		return FALSE;
	}

	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);

	// ������ �������� �����
	m_hButton = CreateWindowW(
		L"button",
		L">>",
		WS_CHILD | WS_VISIBLE,
		m_rectMenu.left + m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// �������� ����������
	if (!m_hButton)
	{
		return FALSE;
	}

	// �������� ScrollBar
	m_hScrollBar = CreateWindowW(
		L"scrollbar",
		NULL,
		WS_CHILD | WS_VISIBLE | SBS_VERT,
		m_rectMenu.left + m_rectMenu.right - WIDTH_BUTTONS, 
		m_rectMenu.top,
		WIDTH_BUTTONS, 
		HEIGHT_BUTTONS,
		hWnd, nullptr, hInst, nullptr);

	// �������� ����������
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

void AreaOfFile::Paint(HDC hdc, PAINTSTRUCT & ps)
{
	if (!m_fileCommander->isLoadedFile(m_numberOfArea))
		return;

	
	INT		FirstPaintingRow;	// ������ �������� ������ (������ �� ������ �������)
	INT		LastPaintingRow;	// ��������� �������� ������ (������ �� ������ �������)

	// ���������� �������� ����� ����� �� �������� �������
	FirstPaintingRow = (ps.rcPaint.top - m_rectData.top) / m_heightChar;
	LastPaintingRow  = (ps.rcPaint.bottom - m_rectData.top) / m_heightChar + 1;

	// ����������� �� ����� �� ������� ���������
	if (FirstPaintingRow < 0)
	{
		FirstPaintingRow = 0;
	}
	if (LastPaintingRow > m_countOfVisibleRows)
	{
		LastPaintingRow = m_countOfVisibleRows + 1;
	}

	SelectObject(hdc, m_hFont);	// �����

	// ����� �������� ����� �� ������ �����
	INT64 numberOfByte = (FirstPaintingRow + m_scrollPos) * LENGTH_OF_BYTE_STRINGS;

	// �������� ���� ������
	COLORREF m_baseTextColor = GetTextColor(hdc);

	// ��������� �����
	StateOfByte	State;
	
	BYTE	Byte							= 0;		// ��������
	CHAR	charOfByte						= 0;		// ���� ��� ������	
	WCHAR	stringOfByte[LENGTH_OF_BYTE]	= { 0 };	// ���� ��� Hex ������

	// ���� �� �������
	for (DWORD NumberRow = FirstPaintingRow; NumberRow < LastPaintingRow; NumberRow++)
	{		
		// ����������� ������ ������
		PaintNumberLine(hdc, NumberRow, NumberRow + m_scrollPos);
		
		// ���� �� ������ � ������
		for (DWORD NumbOfByteInRow = 0; NumbOfByteInRow < LENGTH_OF_BYTE_STRINGS; NumbOfByteInRow++)
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

			// ��������� �����
			PaintByte(hdc, NumberRow, NumbOfByteInRow, stringOfByte, charOfByte);

			numberOfByte++;
			
		}

		// ������� ��������� �����
		SetTextColor(hdc, m_baseTextColor);
		
	}

}

void AreaOfFile::setSize(RECT client)
{
	// ������ ������� � �������
	m_rectData = client;
	m_rectData.top = client.top + HEIGHT_MENU;
	m_rectData.right = client.right - WIDTH_SCROLLBAR;

	// ������ ����
	m_rectMenu = client;
	m_rectMenu.bottom = client.top + HEIGHT_MENU;

	// ��������� ������� � ������� ���� �����
	SetWindowPos(m_hEdit, HWND_BOTTOM,
		m_rectMenu.left, m_rectMenu.top,
		m_rectMenu.right - m_rectMenu.left - WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// ��������� ������� � ������� ������
	SetWindowPos(m_hButton, HWND_BOTTOM,
		m_rectMenu.right - WIDTH_BUTTONS, m_rectMenu.top,
		WIDTH_BUTTONS, HEIGHT_BUTTONS,
		0);

	// ��������� ������� � ������� ScrollBar
	SetWindowPos(m_hScrollBar, HWND_BOTTOM,
		m_rectData.right, m_rectData.top,
		WIDTH_SCROLLBAR, m_rectData.bottom - m_rectData.top,
		0);

	// ��������� ������� ������ � ������������ � ��������
	UpdateFont();

	// ���������� ������ � ��� ScrollBar
	UpdateScrollInfo();
}

/*
void AreaOfFile::setFont(HFONT hFont)
{
	SendMessageW(m_hEdit, WM_SETFONT, (WPARAM)hFont, 1);
}
*/


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
	return m_countOfVisibleRows;
}

void AreaOfFile::SetData(INT64 countRows, double ratioOfScroll, INT maxScrollPos)
{
	m_ratioOfScroll = ratioOfScroll;
	m_maxScrollPos = maxScrollPos;
	m_countRows = countRows;

	UpdateNumberOfRow();
	UpdateFont();
	UpdateScrollInfo();
}

void AreaOfFile::Scroll(INT64 scrollInc)
{
	m_scrollPos += scrollInc;
	ScrollWindowEx(m_hWnd, 0, -m_heightChar * scrollInc, &m_rectData, &m_rectData, NULL, NULL, SW_INVALIDATE);
	SetScrollPos(m_hScrollBar, SB_CTL, m_scrollPos / m_ratioOfScroll + 0.5, true);

	UpdateWindow(m_hWnd);
}



void AreaOfFile::PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView)
{
	WCHAR			m_buffer[LENGTH_OF_BUFFER] = { 0 };
	TextOutW(
		hdc,
		m_rectData.left + m_widthChar * INDENT1,						// �������� �� X
		m_rectData.top + INDENT_OF_TOP + numberLine * m_heightChar,	// �������� �� Y
		m_buffer,													// ������ ������
		wsprintfW(m_buffer, m_format, numberLineForView)				// ����� ������ ������. ���������� ������ ������
	);
}


void AreaOfFile::PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte)
{
	// �������� �� Y
	INT y = m_rectData.top + INDENT_OF_TOP + numberLine * m_heightChar;

	// ����������� ������� �����
	TextOutA(
		hdc,
		m_indentForLetters + m_widthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// ����������� ����� ��� Hex ������
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

	INT CountCharsInRow = INDENT1 + INDENT2 + INDENT3 + m_lengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2) + LENGTH_OF_BYTE_STRINGS + INDENT4;

	INT WidthChar = (m_rectData.right - m_rectData.left) / CountCharsInRow;
	INT HeightChar = (double )WidthChar / FONT_SIZE_RELATION + 0.5;

	if (HeightChar > MAX_FONT_SIZE_HEIGHT)
	{
		WidthChar = MAX_FONT_SIZE_HEIGHT * FONT_SIZE_RELATION;
		HeightChar = MAX_FONT_SIZE_HEIGHT;
	}

	HDC hdc = GetDC(m_hWnd);
	if (!m_hFont)
	{
		DeleteObject(m_hFont);
	}
	m_hFont = CreateFontW(HeightChar,
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

	m_countOfVisibleRows = (m_rectData.bottom - m_rectData.top) / m_heightChar;
}

void AreaOfFile::UpdateNumberOfRow()
{
	INT64 CountRows = m_countRows;
	m_lengthOfNumberRow = 1;
	while ((CountRows /= 16) != 0)
	{
		m_lengthOfNumberRow++;
	}
	wsprintfW(m_format, L"%%0%dlX:", m_lengthOfNumberRow);
	m_lengthOfNumberRow++;	// ���������
	m_indentForBytes = m_rectData.left + m_widthChar * (INDENT1 + INDENT2 + m_lengthOfNumberRow);
	m_indentForLetters = m_rectData.left + m_widthChar * (INDENT1 + INDENT2 + INDENT3 + m_lengthOfNumberRow +
		LENGTH_OF_BYTE_STRINGS * (CharsForByte + INDENT_BETWEEN_BYTES2));
}

