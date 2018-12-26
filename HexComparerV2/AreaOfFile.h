#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18	// ����� ������
#define LENGTH_OF_BYTE 3	// ����� ������, ����������� ����� � ���� ������������������ �����

/// <summary>
/// �������� ����� ������ � �������
/// </summary>
struct DataOfScroll
{
	/// <summary>
	/// ������� ���������� (����� ������ ������������ ������)
	/// </summary>
	INT64			ScrollPos = 0;

	/// <summary>
	/// ����������� ����� �������� �������� ���������� � �������� �������
	/// </summary>
	DOUBLE			RatioOfScroll = 0;

	/// <summary>
	/// ������������ ������� �������
	/// </summary>
	INT				MaxScrollPos = 0;

	/// <summary>
	/// ���������� �������������� �����
	/// </summary>
	INT64			ScrollInc = 0;

	/// <summary>
	/// ������������ ����� ����� (�������� ������� �������)
	/// </summary>
	INT64			CountRows = 0;
};


/// <summary>
/// �����, ���������� �� ������� ����������� ������ �����
/// </summary>
class AreaOfFile
{
private:
	/// <summary>
	/// ����������� ����� ������� �������
	/// </summary>
	INT				m_NumberOfArea					= -1;

	/// <summary>
	/// ��������� �� ������, ���������� �����
	/// </summary>
	FileCommander * m_pFileCommander				= NULL;

	/// <summary>
	/// �������� ����� ������ � �������
	/// </summary>
	DataOfScroll * m_pDataOfScroll					= NULL;

	/// <summary>
	/// ���������� �������� ����
	/// </summary>
	HWND			m_hWnd							= NULL;

	/// <summary>
	/// ���������� ����������
	/// </summary>
	HINSTANCE		m_hInst							= NULL;

	/// <summary>
	/// ���������� ������� � ������� �����
	/// </summary>
	RECT			m_RectData						= { 0 };

	/// <summary>
	/// ���������� ������� � ����
	/// </summary>
	RECT			m_RectMenu						= { 0 };

	/// <summary>
	/// ������ ������� ��� ������ ������
	/// </summary>
	WCHAR			m_Format[LENGTH_OF_BUFFER]		= { 0 };

	/// <summary>
	/// ���������� ���� �����
	/// </summary>
	HWND			m_hEdit							= NULL;

	/// <summary>
	/// ���������� ������
	/// </summary>
	HWND			m_hButton						= NULL;

	/// <summary>
	/// ���������� ������ ����
	/// </summary>
	HWND			m_hScrollBar					= NULL;

	/// <summary>
	/// ���������� ������
	/// </summary>
	HFONT			m_hFont							= NULL;

	/// <summary>
	/// ���������� x ��� ������� � �������
	/// </summary>
	INT				m_IndentForBytes				= 0;

	/// <summary>
	/// ���������� x ��� ������� �������������� ������ ��� ��������
	/// </summary>
	INT				m_IndentForLetters				= 0;
	
	/// <summary>
	/// ������ �������
	/// </summary>
	INT				m_WidthChar						= 0;

	/// <summary>
	/// ������ �������
	/// </summary>
	INT				m_HeightChar					= 0;

	/// <summary>
	/// ����� ������ ������
	/// </summary>
	INT				m_LengthOfNumberRow				= 0;

	/// <summary>
	/// ���������� ������� �����
	/// </summary>
	INT				m_CountOfVisibleRows			= 0;

	/// <summary>
	/// ��������� ������ �������
	/// </summary>
	void			UpdateScrollInfo();

	/// <summary>
	/// ������ ����� ������
	/// </summary>
	/// <param name="hdc">���������� ��������� ����������</param>
	/// <param name="numberLine">����� ������ �� ������</param>
	/// <param name="numberLineForView">�����, ������� ����� ����������</param>
	void inline		PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView);

	/// <summary>
	/// ������ ���� ����
	/// </summary>
	/// <param name="hdc">���������� ��������� ����������</param>
	/// <param name="numberLine">����� ������ �� ������</param>
	/// <param name="numberByte">����� ����� � ������</param>
	/// <param name="stringOfByte">������, ���������� ����������������� �������������</param>
	/// <param name="charOfByte">������ �����</param>
	void inline		PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte);

	/// <summary>
	/// ��������� ����������������� ������������� ��� �����
	/// </summary>
	/// <param name="in">�������� �����</param>
	/// <param name="out">����������������� �������������</param>
	void inline		ByteToHexString(byte in, OUT WCHAR out[]);

	/// <summary>
	/// ��������� �����
	/// </summary>
	void			UpdateFont();

	/// <summary>
	/// ��������� ������ � ������ ������
	/// </summary>
	void			UpdateNumberOfRow();

	/// <summary>
	/// ��������� ����������� ������ �������� �����
	/// </summary>
	/// <param name="file">����� ��� ����� �����</param>
	/// <returns></returns>
	BOOL			OpenFileDialog(LPWSTR file);

	/// <summary>
	/// ������ ����� ������ �������
	/// </summary>
	/// <param name="hdc">���������� ��������� ����������</param>
	void			PaintBorder(HDC hdc);


public:
	
	/// <summary>
	/// �������������� ������
	/// </summary>
	/// <param name="number">����� �������</param>
	/// <param name="hWnd">���������� �������� ����</param>
	/// <param name="hInst">���������� ����������</param>
	/// <param name="hFont">���������� ������ (����������� ��� edit)</param>
	/// <param name="fileCommander">��������� �� ������, ���������� �����</param>
	/// <param name="m_DateOfScroll">��������� �� ������, ���������� ������ � �������, ����� ��� ���� ��������</param>
	/// <returns>���������� �������������</returns>
	BOOL		Initialize(INT number, HWND hWnd, HINSTANCE hInst, 
					HFONT hFont, 
					FileCommander * fileCommander, 
					DataOfScroll * m_DateOfScroll);

	/// <summary>
	/// �������� ���� �������� ������������
	/// </summary>
	void		CloseHandle();

	/// <summary>
	/// ��������� ���� �������
	/// </summary>
	/// <param name="hdc">�������� ���������� ��� ���������</param>
	/// <param name="ps">���������, ���������� �������� � ���������</param>
	void		PaintArea(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// ������������� ����� ������ �������
	/// </summary>
	/// <param name="client">�������� ����� ������ �������</param>
	void		setSize(RECT client);

	/// <summary>
	/// ���������� ���������� ������
	/// </summary>
	/// <returns>���������� ������</returns>
	HWND		GetButton();

	/// <summary>
	/// ���������� ���������� ���� �����
	/// </summary>
	/// <returns>���������� ���� �����</returns>
	HWND		GetEdit();

	/// <summary>
	/// ���������� ���������� ������ ����
	/// </summary>
	/// <returns>���������� ������ ����</returns>
	HWND		GetScrollBar();

	/// <summary>
	/// ���������� ����� ������� �����
	/// </summary>
	/// <returns>����� ������� �����</returns>
	INT			GetCountOfVisibleRows();

	/// <summary>
	/// ������������� ��� ������
	/// </summary>
	void		UpdateData();

	/// <summary>
	/// ��������� ��������� ����������� �������
	/// </summary>
	void		Scroll();

	/// <summary>
	/// ��������� ����, ���� hEdit ����������� �������
	/// </summary>
	/// <param name="hEdit">���������� ����</param>
	void		�hangeEdit(HWND hEdit);

	/// <summary>
	/// ��������� ����, ���� hButton ����������� �������
	/// </summary>
	/// <param name="hButton"></param>
	void		�lickButton(HWND hButton);

	/// <summary>
	/// �������� ����� �� ���� �� edit
	/// </summary>
	void		OpenFile();
};

