#pragma once
#include "stdafx.h"

/// <summary>
/// �������� ��� ������ � ������
/// </summary>
class FileMapping
{

public:

	/// <summary>
	/// ��������� ����
	/// </summary>
	/// <param name="fileName">���� � ����� </param>
	/// <returns></returns>
	BOOL		OpenFile(LPCWSTR fileName);

	/// <summary>
	/// ���������� ����� �������� �����
	/// </summary>
	/// <param name="numberOfByte">����� �����</param>
	/// <returns>���������� �����������</returns>
	BOOL		MapView(INT64 numberOfByte);

	/// <summary>
	/// ��������� ����� �� ��� ������
	/// </summary>
	/// <param name="numberOfByte">����� �����</param>
	/// <param name="Byte">����������, � ������� ������������ �������� �����</param>
	/// <returns>���������� �������</returns>
	BOOL		GetByte(INT64 numberOfByte, OUT BYTE & Byte);

	/// <summary>
	/// ���������� ������ �����
	/// </summary>
	/// <returns>������ �����</returns>
	INT64		GetSizeOfFile();

	/// <summary>
	/// ��������� ���� � ��� ��� �����������
	/// </summary>
	void		CloseFile();

	/// <summary>
	/// ��������� ����
	/// </summary>
	~FileMapping();

private:
	/// <summary>
	/// ��������� �� ����������� �����
	/// </summary>
	LPVOID		m_pMapViewOfFile	=	NULL;

	/// <summary>
	/// ���������� �����
	/// </summary>
	HANDLE		m_hFile				=	NULL;

	/// <summary>
	/// ���������� ��������
	/// </summary>
	HANDLE		m_hFileMapping		=	NULL;

	/// <summary>
	/// ������ �����
	/// </summary>
	INT64		m_SizeOfFile		=	-1;

	/// <summary>
	/// ������ ������� �������� �����������
	/// </summary>
	INT64		m_BeginPage			=	0;
};

