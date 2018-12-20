#pragma once
#include "stdafx.h"
#include "FileMapping.h"

typedef enum StateOfByte
{
	FileNotLoaded,
	FileEnded,
	ByteEqual,
	ByteNotEqual
};

class FileCommander
{
private:
	FileMapping		m_fileMappings[COUNT_OF_FILES];
	INT64			m_currentNumberByte						= 0;
	BYTE			m_currentBytes[COUNT_OF_FILES]			= { 0 };
	BOOL			m_isLoadedFiles[COUNT_OF_FILES]			= { 0 };
	StateOfByte		m_currentStateOfBytes[COUNT_OF_FILES];

	void			Compare(INT64 numberOfByte);
public:
	BOOL			LoadFile(int indexFile, LPCWSTR fileName);
	void			CloseFile(int indexFile);
	StateOfByte		getByte(int indexFile, INT64 numberOfByte, OUT BYTE & b);
	INT64			getMaxSize();
};

