#pragma once
#include "stdafx.h"
#include "FileMapping.h"

typedef enum StateOfByte
{
	FileNtLoaded,
	FileEnded,
	ByteEqual,
	ByteNotEqual
};

class FileCommander
{
private:
	/// <summary>
	/// 
	/// </summary>
	FileMapping		m_fileMappings[COUNT_OF_FILES];

	INT64			m_currentNumberByte						= -1;
	BYTE			m_currentBytes[COUNT_OF_FILES]			= { 0 };
	BOOL			m_isLoadedFiles[COUNT_OF_FILES]			= { 0 };
	StateOfByte		m_currentStateOfBytes[COUNT_OF_FILES];

	void			Compare(INT64 numberOfByte);
public:
					FileCommander();
	BOOL			LoadFile(INT indexFile, LPCWSTR fileName);
	BOOL			isLoadedFile(INT indexfile);
	void			CloseFile(INT indexFile);
	StateOfByte		getByte(INT indexFile, INT64 numberOfByte, OUT BYTE & b);
	INT64			getMaxSize();
};

