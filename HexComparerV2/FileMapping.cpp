
#include "stdafx.h"
#include "FileMapping.h"



FileMapping::FileMapping()
{
	m_sizeOfFile = -1;
}

BOOL FileMapping::OpenFile(LPCWSTR fileName)
{
	// �������� �����
	m_hFile = CreateFileW(fileName, 
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)	// �������� ����������
	{
		return FALSE;
	}

	// �������� ��������
	m_hFileMapping = CreateFileMappingW(m_hFile, NULL,
				PAGE_READONLY, 
				0, 0,
				NULL);
	if ((m_hFileMapping == INVALID_HANDLE_VALUE) || (m_hFileMapping == NULL))	// �������� ����������
	{
		return FALSE;
	}
	
	m_mapViewOfFile = MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (m_mapViewOfFile == NULL)	// �������� ����������
	{
		return FALSE;
	}

	// ��������� ������� �����
	LARGE_INTEGER FileSize = { 0 };
	GetFileSizeEx(m_hFile, &FileSize);
	m_sizeOfFile = FileSize.QuadPart;

	return TRUE;
}

BOOL FileMapping::getByte(INT64 numberOfByte, OUT BYTE & Byte)
{
	// �������� �� ����
	if (numberOfByte >= m_sizeOfFile)
	{
		return FALSE;
	}

	// ������ �����
	Byte = ((byte*)m_mapViewOfFile)[numberOfByte];

	return TRUE;
}

INT64 FileMapping::getSizeOfFile()
{
	return m_sizeOfFile;
}

void FileMapping::CloseFile()
{
	// ��������� 
	m_sizeOfFile = -1;

	// �������� �����������
	if (m_mapViewOfFile != NULL)
	{
		UnmapViewOfFile(m_mapViewOfFile);
		m_mapViewOfFile = NULL;
	}

	// �������� ��������
	if ((m_hFileMapping != NULL) || (m_hFileMapping != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}

	// �������� �����
	if ((m_hFile != NULL) || (m_hFile != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}



FileMapping::~FileMapping()
{
	CloseFile();
}
