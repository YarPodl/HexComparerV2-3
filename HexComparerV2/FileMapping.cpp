
#include "stdafx.h"
#include "FileMapping.h"



FileMapping::FileMapping()
{
	m_sizeOfFile = -1;
}

BOOL FileMapping::OpenFile(LPCWSTR fileName)
{
	// Открытие файла
	m_hFile = CreateFileW(fileName, 
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)	// Проверка успешности
	{
		return FALSE;
	}

	// Создание маппинга
	m_hFileMapping = CreateFileMappingW(m_hFile, NULL,
				PAGE_READONLY, 
				0, 0,
				NULL);
	if ((m_hFileMapping == INVALID_HANDLE_VALUE) || (m_hFileMapping == NULL))	// Проверка успешности
	{
		return FALSE;
	}
	
	m_mapViewOfFile = MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (m_mapViewOfFile == NULL)	// Проверка успешности
	{
		return FALSE;
	}

	// Получение размера файла
	LARGE_INTEGER FileSize = { 0 };
	GetFileSizeEx(m_hFile, &FileSize);
	m_sizeOfFile = FileSize.QuadPart;

	return TRUE;
}

BOOL FileMapping::getByte(INT64 numberOfByte, OUT BYTE & Byte)
{
	// Кончился ли файл
	if (numberOfByte >= m_sizeOfFile)
	{
		return FALSE;
	}

	// Чтение байта
	Byte = ((byte*)m_mapViewOfFile)[numberOfByte];

	return TRUE;
}

INT64 FileMapping::getSizeOfFile()
{
	return m_sizeOfFile;
}

void FileMapping::CloseFile()
{
	// Обнуление 
	m_sizeOfFile = -1;

	// Закрытие отображения
	if (m_mapViewOfFile != NULL)
	{
		UnmapViewOfFile(m_mapViewOfFile);
		m_mapViewOfFile = NULL;
	}

	// Закрытие маппинга
	if ((m_hFileMapping != NULL) || (m_hFileMapping != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}

	// Закрытие файла
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
