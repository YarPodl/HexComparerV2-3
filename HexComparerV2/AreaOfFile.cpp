#include "AreaOfFile.h"



AreaOfFile::AreaOfFile()
{
}


AreaOfFile::~AreaOfFile()
{
}

void AreaOfFile::Paint(HDC hdc, PAINTSTRUCT & ps)
{
	INT64		firstPaintingRow;	// ������ �������� ������ (������ �� ������ �������)
	INT64		lastPaintingRow;	// ��������� �������� ������ (������ �� ������ �������)

	// ���������� �������� ����� ����� ���������� �������
	firstPaintingRow = (ps.rcPaint.top - m_rectData.top) / heightChar;
	lastPaintingRow  = (ps.rcPaint.bottom - m_rectData.top) / heightChar + 1;

	// ����������� �� ����� �� ������� ���������
	if (firstPaintingRow < 0)
	{
		firstPaintingRow = 0;
	}
	if (lastPaintingRow >= m_countOfVisibleRows)
	{
		lastPaintingRow = m_countOfVisibleRows - 1;
	}

	SelectObject(hdc, m_hFont);	// �����

	// ����� �������� ����� �� ������ �����
	INT64 numberOfByte = firstPaintingRow * LENGTH_OF_BYTE_STRINGS;

	// ���� ������
	SetTextColor(hdc, m_baseTextColor);

	// ����������� ������� �����
	for (int NumberRow = firstPaintingRow; NumberRow < lastPaintingRow; NumberRow++)
	{
		PaintNumberLine(hdc, NumberRow, NumberRow + m_scrollPos);
	}

	// ��������� �����
	StateOfByte	State;
	// ��������
	BYTE		Byte							= 0;
	// ���� ��� ������
	char		charOfByte						= 0;
	// ���� ��� Hex ������
	WCHAR		stringOfByte[LENGTH_OF_BYTE]	= { 0 };

	// ���� �� �������
	for (int NumberRow = firstPaintingRow; NumberRow < lastPaintingRow; NumberRow++)
	{
		// ���� �� ������ � ������
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
			// ��������� �����
			PaintByte(hdc, NumberRow, NumbOfByteInRow, stringOfByte, charOfByte);

			numberOfByte++;
		}
	}
	SetTextColor(hdc, m_baseTextColor);
}



void AreaOfFile::PaintNumberLine(HDC hdc, int numberLine, INT64 numberLineForView)
{
	TextOutW(
		hdc,
		m_rectData.left + widthChar * INDENT1,						// �������� �� X
		m_rectData.top + INDENT_OF_TOP + numberLine * heightChar,	// �������� �� Y
		m_buffer,													// ������ ������
		wsprintfW(m_buffer, format, numberLineForView)				// ����� ������ ������. ���������� ������ ������
	);
}


void AreaOfFile::PaintByte(HDC hdc, int numberLine, int numberByte, WCHAR stringOfByte[], char charOfByte)
{
	// �������� �� Y
	int y = m_rectData.top + INDENT_OF_TOP + numberLine * heightChar;

	// ����������� ������� �����
	TextOutA(
		hdc,
		indentForLetters + widthChar * numberByte,
		y,
		(LPCSTR)&charOfByte,
		1
	);

	// ����������� ����� ��� Hex ������
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
	SCROLLINFO scrollInfo;
	m_countOfVisibleRows = (m_rectData.bottom - m_rectData.top) / heightChar;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.nMin = 0;
	scrollInfo.nMax = m_countRows > m_countOfVisibleRows ? maxScrollPos : 0;
	scrollInfo.nPage = m_countOfVisibleRows;
	scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
	SetScrollInfo(m_scrollBar, SB_CTL, &scrollInfo, TRUE);
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

