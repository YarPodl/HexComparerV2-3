#include "FileCommander.h"



void FileCommander::Compare(INT64 numberOfByte)
{
	int PrevByte = -1;	// �������� ����� ����������� ��������� ����� (-1 - ������ ��� ������ �������� ����)
	BOOL Equal = TRUE;	// ����� �� ����� ������ � �����

	for (int NumbOfFile = 0; NumbOfFile < COUNT_OF_FILES; NumbOfFile++)
	{
		// �������� �� ����
		if (m_fileMappings[NumbOfFile].getByte(numberOfByte, OUT m_currentBytes[NumbOfFile]))
		{
			// ���� ������� ��������
			if ((PrevByte != -1) && (m_currentBytes[NumbOfFile] != PrevByte))
			{
				Equal = FALSE;
			}
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
		for (int numbOfFile = 0; numbOfFile < COUNT_OF_FILES; numbOfFile++)
		{
			if (m_currentStateOfBytes[numbOfFile] == ByteEqual)
			{
				m_currentStateOfBytes[numbOfFile] = ByteNotEqual;
			}
		}
	}
}


BOOL FileCommander::LoadFile(int indexFile, LPCWSTR fileName)
{
	if (m_fileMappings[indexFile].OpenFile(fileName))
	{
		m_isLoadedFiles[indexFile] = TRUE;
		return TRUE;
	}
	m_isLoadedFiles[indexFile] = FALSE;
	return FALSE;
}

void FileCommander::CloseFile(int indexFile)
{
	m_isLoadedFiles[indexFile] = FALSE;
	m_fileMappings[indexFile].CloseFile();
}

StateOfByte FileCommander::getByte(int indexFile, INT64 numberOfByte, OUT BYTE & b)
{
	b = 0;

	// ���� �� ��� �������� ����
	if (!m_isLoadedFiles[indexFile])	
	{
		return FileNotLoaded;
	}

	// ���� ����� � ������ ������� ��� �� ���������������
	if (numberOfByte != m_currentNumberByte)
	{
		Compare(numberOfByte);
	}

	b = m_currentBytes[indexFile];

	return m_currentStateOfBytes[indexFile];
}

INT64 FileCommander::getMaxSize()
{
	INT64 maxSizeOfFile;
	INT64 sizeOfFile;
	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		sizeOfFile = m_fileMappings[i].getSizeOfFile();
		if (sizeOfFile > maxSizeOfFile)
		{
			maxSizeOfFile = sizeOfFile;
		}
	}
	return maxSizeOfFile;
}
