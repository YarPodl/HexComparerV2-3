#pragma once
#include "stdafx.h"
#include "FileMapping.h"

#define DIFFERENCE_NOT_FOUND -1
#define SEARCH_CANCELED -2

/// <summary>
/// ��������� ������
/// </summary>
enum StateOfByte
{
	FileNtLoaded,		// ���� �� ��������
	FileEnded,			// ���� ����������
	ByteEqual,			// ���� ����� ���������
	ByteNotEqual		// ���� �� �����
};

/// <summary>
/// ������������ ������ � ������� � �� ���������
/// </summary>
class FileCommander
{

public:

	/// <summary>
	/// ��������� ����
	/// </summary>
	/// <param name="indexFile">������, ��� �������� ���������� ������� ����</param>
	/// <param name="fileName">���� � ����� � ��� ���</param>
	/// <returns>���������� ��������</returns>
	BOOL			LoadFile(INT indexFile, LPCWSTR fileName);

	/// <summary>
	/// ��������� �������� �� ����
	/// </summary>
	/// <param name="indexfile">������ ����� ��� ��������</param>
	/// <returns>TRUE, ���� ���� ��������</returns>
	BOOL			IsLoadedFile(INT indexfile);

	/// <summary>
	/// ��������� ���� �� �������
	/// </summary>
	/// <param name="indexFile">������ ������������ �����</param>
	void			CloseFile(INT indexFile);

	/// <summary>
	/// ��������� �������� ����� � ��� ���������
	/// </summary>
	/// <param name="indexFile">������ �����, ���� �������� ���������� ��������</param>
	/// <param name="numberOfByte">����� �����</param>
	/// <param name="b">����������, � ������� ������������ �������� �����</param>
	/// <returns>��������� �����</returns>
	StateOfByte		GetByte(INT indexFile, INT64 numberOfByte, OUT BYTE & b);

	/// <summary>
	/// ����� ������ (����������� ������������) �������� � ������ � �������� ������� � �������� �����
	/// </summary>
	/// <param name="beginOfSearch">������� ������ ������</param>
	/// <param name="step">���, � ������� ���� ����� (�������������� 1 ��� -1)</param>
	/// <param name="cancel">���� ����� false, ����� ����������</param>
	/// <returns>������� ���������� ��������, ���� -1 ���� �������� �� �������, ���� -2 ���� ����������</returns>
	INT64			FindDifference(INT64 beginOfSearch, INT step, BOOL & cancel);

	/// <summary>
	/// ���������� ������ �����
	/// </summary>
	/// <returns>������ �����</returns>
	INT64			GetMaxSize();

private:
	/// <summary>
	/// ������ ��������, ���������� � �������
	/// </summary>
	FileMapping		m_FileMappings[COUNT_OF_FILES];

	/// <summary>
	/// ����� �������� �����
	/// </summary>
	INT64			m_CurrentNumberByte				= -1;

	/// <summary>
	/// �������� ������� ������ ��� ���� ������
	/// </summary>
	BYTE			m_CurrentBytes[COUNT_OF_FILES]	= { 0 };

	/// <summary>
	/// ��������� �� �����
	/// </summary>
	BOOL			m_IsLoadedFiles[COUNT_OF_FILES]	= { 0 };

	/// <summary>
	/// ��������� ������� ������
	/// </summary>
	StateOfByte		m_CurrentStateOfBytes[COUNT_OF_FILES];

	/// <summary>
	/// ����� �� ������� �����
	/// </summary>
	BOOL			m_CurrentEqual					= FALSE;

	/// <summary>
	/// ���������� �����, ��������� m_CurrentStateOfBytes � m_CurrentBytes ��� �������� ������ �����
	/// </summary>
	/// <param name="numberOfByte">����� �����</param>
	/// <returns>����� �� �����</returns>
	BOOL			Compare(INT64 numberOfByte);

};

