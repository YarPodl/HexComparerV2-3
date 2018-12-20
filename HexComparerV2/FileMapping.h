#pragma once
#include "stdafx.h"

class FileMapping
{
private:

	LPVOID		m_mapViewOfFile	=	NULL;
	HANDLE		m_hFile				=	NULL;
	HANDLE		m_hFileMapping		=	NULL;
	INT64		m_sizeOfFile		=	-1;

public:
				FileMapping();
	BOOL		OpenFile(LPCWSTR fileName);
	BOOL		getByte(INT64 numberOfByte, OUT BYTE & Byte);
	INT64		getSizeOfFile();
	void		CloseFile();
				~FileMapping();
};

