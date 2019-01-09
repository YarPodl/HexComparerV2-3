#pragma once
#include "stdafx.h"

#include "AreaOfFile.h"
#include "FileCommander.h"


/// <summary>
/// ������������ ������ ���� ��������� ���������� �������
/// </summary>
class DisplayArea
{
private:

	/// <summary>
	/// �������� �������, ���������� �� ������� ������
	/// </summary> 
	AreaOfFile		m_AreasOfFiles[COUNT_OF_FILES];

	/// <summary>
	/// �������� �� �����
	/// </summary>
	FileCommander	m_FileCommander;

	/// <summary>
	/// ���������� �������� ����
	/// </summary>
	HWND			m_hWnd				= NULL;
	 
	/// <summary>
	/// ���������� ����������
	/// </summary>
	HINSTANCE		m_hInst				= NULL;

	/// <summary>
	/// �������� ����� ������ � �������
	/// </summary>
	DataOfScroll	m_DataOfScroll		= { 0 };
	 
	/// <summary>
	/// ������������ ���������� ���� ����� �������� ������
	/// </summary>
	INT64			m_CountOfByte		= 0;
	 
	/// <summary>
	/// ������ ���������� ������� ����
	/// </summary>
	INT				m_HeightClient		= 0;
	 
	/// <summary>
	/// ������ ���������� ������� ����
	/// </summary>
	INT				m_WidthClient		= 0;
	 
	/// <summary>
	/// ����������� �������� ���������� ������� ����� ��� ���� ��������
	/// </summary>
	INT				m_MinCountOfVisibleRows = 0;
	 
	/// <summary>
	/// ������� �����
	/// </summary>
	HFONT			m_hFont				= NULL;

	/// <summary>
	/// ������ ������ ���������� �������
	/// </summary>
	HWND			m_hButtonNext		= NULL;

	/// <summary>
	/// ������ ������ ����������� �������
	/// </summary>
	HWND			m_hButtonPrev		= NULL;

	/// <summary>
	/// ������ ������ ���������� �������
	/// </summary>
	HWND			m_hButtonSearch		= NULL;

	/// <summary>
	/// ���������, �������� �������� m_ScrollInc
	/// </summary>
	void			Scroll();

	/// <summary>
	/// ��������� ����� �������� � ���������� ��� ���� ��������
	/// </summary>
	void			SetSizeAreaOfFile();

	/// <summary>
	/// ���������� ���� ����������
	/// </summary>
	void			UpdateData();

	void			NextDifference();
	void			PrevDifference();
	void			CountDifference();

public:

	/// <summary>
	/// �������������
	/// </summary>
	/// <param name="hWnd">���������� ����</param>
	/// <param name="hInst">���������� ����������</param>
	DisplayArea(HWND hWnd, HINSTANCE hInst);

	/// <summary>
	/// ��������� ��� Handles
	/// </summary>
	~DisplayArea();

	/// <summary>
	/// ��������� �������� ����
	/// </summary>
	/// <param name="lParam">�������� ����� ������� ���������� ������� ����</param>
	void			ChangeSize(LPARAM lParam);

	/// <summary>
	/// ���������� ��������� ���� ��������
	/// </summary>
	/// <param name="hdc">�������� ���������� ��� ���������</param>
	/// <param name="ps">���������, ���������� �������� � ���������</param>
	void			Paint(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// ������ ����� �� ���� ������
	/// </summary>
	void			ScrollLineUp();

	/// <summary>
	/// ������ ���� �� ���� ������
	/// </summary>
	void			ScrollLineDown();

	/// <summary>
	/// ������ ����� �� ���� ��������
	/// </summary>
	/// <param name="lParam">�������� ���������� ���������� (��������� NULL)</param>
	void			ScrollPageUp(LPARAM lParam);

	/// <summary>
	/// ������ ���� �� ���� ��������
	/// </summary>
	/// <param name="lParam">�������� ���������� ���������� (��������� NULL)</param>
	void			ScrollPageDown(LPARAM lParam);


	/// <summary>
	/// ������ � ������
	/// </summary>
	void			ScrollBegin();

	/// <summary>
	/// ������ � �����
	/// </summary>
	void			ScrollEnd();

	/// <summary>
	/// ������ �������� � ���������� �������
	/// </summary>
	/// <param name="lParam">�������� ���������� ����������</param>
	void			ScrollTo(LPARAM lParam);
	
	/// <summary>
	/// ���������� ���������� ������� ����� � �������, ������� ����������� ���������
	/// </summary>
	/// <param name="lParam">�������� ���������� ���������� (��������� NULL)</param>
	INT				GetCountOfVisibleRows(LPARAM lParam);

	/// <summary>
	/// ��������� ����������� ���������� ������� ����� ��� ��������
	/// </summary>
	void			UpdateMinCountOfVisibleRows();

	/// <summary>
	/// ��������� ���� �� ���� �� edit, ������� ��������� � ������ �����
	/// </summary>
	/// <returns>��� �� ������ ����</returns>
	bool			OpenFileFromEdit();

	/// <summary>
	/// ��������� ���� ��� ����������� edit
	/// </summary>
	/// <param name="hEdit">�������� ��������� ���� �����, ������� ���� ��������</param>
	void			�hangeEdit(HWND hEdit);

	/// <summary>
	/// ��������� ������ �������� �����
	/// </summary>
	/// <param name="hButton">�������� ��������� ������� ������</param>
	void			�lickButton(HWND hButton);
};

