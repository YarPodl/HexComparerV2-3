#include "stdafx.h"
#include "FileCommander.h"



void FileCommander::Compare(INT64 numberOfByte)
{
	INT		PrevByte	= -1;	// �������� ����� ����������� ��������� ����� (-1 - ������ ��� ������ �������� ����)
	BOOL	Equal		= TRUE;	// ����� �� ����� ������ � �����

	// ���� �� ������
	for (INT NumbOfFile = 0; NumbOfFile < COUNT_OF_FILES; NumbOfFile++)
	{
		// �������� �� ����
		if (!m_IsLoadedFiles[NumbOfFile])
		{
			m_CurrentStateOfBytes[NumbOfFile] = FileNtLoaded;
		}

		// �������� �� ����
		else if (m_FileMappings[NumbOfFile].getByte(numberOfByte, OUT m_CurrentBytes[NumbOfFile]))
		{
			// ���� ������� ��������
			if ((PrevByte != -1) && (m_CurrentBytes[NumbOfFile] != PrevByte))
			{
				Equal = FALSE;
			}

			PrevByte = m_CurrentBytes[NumbOfFile];
			m_CurrentStateOfBytes[NumbOfFile] = ByteEqual;
		}
		else
		{
			Equal = FALSE;
			m_CurrentBytes[NumbOfFile] = 0;
			m_CurrentStateOfBytes[NumbOfFile] = FileEnded;
		}
	}

	// ���� ���� �������� - ��������� ����� ��������� ���� ��������� ������
	if (!Equal)
	{
		for (INT numbOfFile = 0; numbOfFile < COUNT_OF_FILES; numbOfFile++)
		{
			if (m_CurrentStateOfBytes[numbOfFile] == ByteEqual)
			{
				m_CurrentStateOfBytes[numbOfFile] = ByteNotEqual;
			}
		}
	}
}


BOOL FileCommander::LoadFile(INT indexFile, LPCWSTR fileName)
{
	if (m_FileMappings[indexFile].OpenFile(fileName))
	{
		m_IsLoadedFiles[indexFile] = TRUE;
		return TRUE;
	}

	m_IsLoadedFiles[indexFile] = FALSE;

	return FALSE;
}


BOOL FileCommander::IsLoadedFile(INT indexfile)
{
	return m_IsLoadedFiles[indexfile];
}


void FileCommander::CloseFile(INT indexFile)
{
	m_IsLoadedFiles[indexFile] = FALSE;
	m_FileMappings[indexFile].CloseFile();
}


StateOfByte FileCommander::GetByte(INT indexFile, INT64 numberOfByte, OUT BYTE & Byte)
{
	Byte = 0;

	// ���� ����� � ������ ������� ��� �� ���������������
	if (numberOfByte != m_CurrentNumberByte)
	{
		Compare(numberOfByte);
	}

	Byte = m_CurrentBytes[indexFile];

	return m_CurrentStateOfBytes[indexFile];
}


INT64 FileCommander::GetMaxSize()
{
	INT64	MaxSizeOfFile	= 0;	// ������������ ������ �����
	INT64	SizeOfFile		= 0;	// ������ �����

	// ����� ������������� �������
	for (INT i = 0; i < COUNT_OF_FILES; i++)
	{
		SizeOfFile = m_FileMappings[i].getSizeOfFile();
		if (SizeOfFile > MaxSizeOfFile)
		{
			MaxSizeOfFile = SizeOfFile;
		}
	}

	return MaxSizeOfFile;
}
