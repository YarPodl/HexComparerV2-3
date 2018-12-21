#include "FileCommander.h"



void FileCommander::Compare(INT64 numberOfByte)
{
	INT		PrevByte	= -1;	// �������� ����� ����������� ��������� ����� (-1 - ������ ��� ������ �������� ����)
	BOOL	Equal		= TRUE;	// ����� �� ����� ������ � �����

	for (INT NumbOfFile = 0; NumbOfFile < COUNT_OF_FILES; NumbOfFile++)
	{
		if (!isLoadedFile(NumbOfFile))
		{
			m_currentStateOfBytes[NumbOfFile] = FileNtLoaded;
		}
		// �������� �� ����
		else if (m_fileMappings[NumbOfFile].getByte(numberOfByte, OUT m_currentBytes[NumbOfFile]))
		{
			// ���� ������� ��������
			if ((PrevByte != -1) && (m_currentBytes[NumbOfFile] != PrevByte))
			{
				Equal = FALSE;
			}
			PrevByte = m_currentBytes[NumbOfFile];
			m_currentStateOfBytes[NumbOfFile] = ByteEqual;
		}
		else
		{
			Equal = FALSE;
			m_currentBytes[NumbOfFile] = 0;
			m_currentStateOfBytes[NumbOfFile] = FileEnded;
		}
	}

	// ���� ���� �������� - ��������� ����� ��������� ���� ��������� ������
	if (!Equal)
	{
		for (INT numbOfFile = 0; numbOfFile < COUNT_OF_FILES; numbOfFile++)
		{
			if (m_currentStateOfBytes[numbOfFile] == ByteEqual)
			{
				m_currentStateOfBytes[numbOfFile] = ByteNotEqual;
			}
		}
	}
}


FileCommander::FileCommander()
{

}

BOOL FileCommander::LoadFile(INT indexFile, LPCWSTR fileName)
{
	if (m_fileMappings[indexFile].OpenFile(fileName))
	{
		m_isLoadedFiles[indexFile] = TRUE;
		return TRUE;
	}

	m_isLoadedFiles[indexFile] = FALSE;

	return FALSE;
}

BOOL FileCommander::isLoadedFile(INT indexfile)
{
	return m_isLoadedFiles[indexfile];
}

void FileCommander::CloseFile(INT indexFile)
{
	m_isLoadedFiles[indexFile] = FALSE;
	m_fileMappings[indexFile].CloseFile();
}

StateOfByte FileCommander::getByte(INT indexFile, INT64 numberOfByte, OUT BYTE & Byte)
{
	Byte = 0;

	// ���� ����� � ������ ������� ��� �� ���������������
	if (numberOfByte != m_currentNumberByte)
	{
		Compare(numberOfByte);
	}

	Byte = m_currentBytes[indexFile];

	return m_currentStateOfBytes[indexFile];
}

INT64 FileCommander::getMaxSize()
{
	INT64	MaxSizeOfFile	= 0;
	INT64	SizeOfFile		= 0;

	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		SizeOfFile = m_fileMappings[i].getSizeOfFile();
		if (SizeOfFile > MaxSizeOfFile)
		{
			MaxSizeOfFile = SizeOfFile;
		}
	}

	return MaxSizeOfFile;
}
