#include "FileCommander.h"



void FileCommander::Compare(INT64 numberOfByte)
{
	int PrevByte = -1;	// Значение байта предыдущего открытого файла (-1 - значит это первый открытый файл)
	BOOL Equal = TRUE;	// Равны ли байты файлов в итоге

	for (int NumbOfFile = 0; NumbOfFile < COUNT_OF_FILES; NumbOfFile++)
	{
		// Кончился ли файл
		if (m_fileMappings[NumbOfFile].getByte(numberOfByte, OUT m_currentBytes[NumbOfFile]))
		{
			// Если найдено различие
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

	// Если есть различие - установка этого состояния всем доступным байтам
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


FileCommander::FileCommander()
{
	//m_isLoadedFiles[0] = 1;
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

BOOL FileCommander::isLoadedFile(int indexfile)
{
	return m_isLoadedFiles[indexfile];
}

void FileCommander::CloseFile(int indexFile)
{
	m_isLoadedFiles[indexFile] = FALSE;
	m_fileMappings[indexFile].CloseFile();
}

StateOfByte FileCommander::getByte(int indexFile, INT64 numberOfByte, OUT BYTE & Byte)
{
	Byte = 0;

	// Если байты с данным номером еще не анализировались
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

	for (int i = 0; i < COUNT_OF_FILES; i++)
	{
		SizeOfFile = m_fileMappings[i].getSizeOfFile();
		if (SizeOfFile > MaxSizeOfFile)
		{
			MaxSizeOfFile = SizeOfFile;
		}
	}

	return MaxSizeOfFile;
}
