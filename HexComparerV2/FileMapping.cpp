
#include "stdafx.h"
#include "FileMapping.h"



BOOL FileMapping::OpenFile(LPCWSTR fileName)
{
	// Открытие файла
	m_hFile = CreateFileW(fileName, 
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);

	// Проверка успешности
	if (m_hFile == INVALID_HANDLE_VALUE)	
	{
		return FALSE;
	}

	// Создание маппинга
	m_hFileMapping = CreateFileMappingW(m_hFile, NULL,
				PAGE_READONLY, 
				0, 0,
				NULL);

	// Проверка успешности
	if ((m_hFileMapping == INVALID_HANDLE_VALUE) || (m_hFileMapping == NULL))	
	{
		return FALSE;
	}
	
	// Создание отображения
	m_pMapViewOfFile = MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, 0);
	// Проверка успешности
	if (m_pMapViewOfFile == NULL)	
	{
		return FALSE;
	}

	// Получение размера файла
	LARGE_INTEGER FileSize = { 0 };
	GetFileSizeEx(m_hFile, &FileSize);
	m_SizeOfFile = FileSize.QuadPart;

	return TRUE;
}

BOOL FileMapping::getByte(INT64 numberOfByte, OUT BYTE & Byte)
{
	// Кончился ли файл
	if (numberOfByte >= m_SizeOfFile)
	{
		return FALSE;
	}

	// Чтение байта
	Byte = ((byte*)m_pMapViewOfFile)[numberOfByte];

	return TRUE;
}

INT64 FileMapping::getSizeOfFile()
{
	return m_SizeOfFile;
}

void FileMapping::CloseFile()
{
	// Обнуление 
	m_SizeOfFile = -1;

	// Закрытие отображения
	if (m_pMapViewOfFile != NULL)
	{
		UnmapViewOfFile(m_pMapViewOfFile);
		m_pMapViewOfFile = NULL;
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
