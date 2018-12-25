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
	/// ����� ���������� �����
	/// </summary>
	INT64			m_CountRows			= 0;

	/// <summary>
	/// ������� ���������� (����� ������ ������������ ������)
	/// </summary>
	INT64			m_ScrollPos			= 0;
	
	/// <summary>
	/// ���������� �������������� �����
	/// </summary>
	INT64			m_ScrollInc			= 0;
	 
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
	/// ����������� ����� �������� �������� ���������� � �������� �������
	/// </summary>
	double			m_RatioOfScroll		= 0;
	 
	/// <summary>
	/// ������� �����
	/// </summary>
	HFONT			m_hFont				= NULL;
	 
	/// <summary>
	/// ������������ ������� �������
	/// </summary>
	INT				m_MaxScrollPos		= 0;

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
	/// ���������� ���������� ������� �����
	/// </summary>
	/// <param name="lParam">�������� ���������� ���������� (��������� NULL)</param>
	INT				GetCountOfVisibleRows(LPARAM lParam);

	/// <summary>
	/// ��������� ���� �� ���� �� edit, ������� ��������� � ������ �����
	/// </summary>
	/// <returns>��� �� ������ ����</returns>
	bool			OpenFileFromEdit();

	/// <summary>
	/// ��������� ���� ��� ����������� edit
	/// </summary>
	/// <param name="lParam">�������� ��������� ���� �����, ������� ���� ��������</param>
	void			�hangeEdit(LPARAM lParam);

	/// <summary>
	/// ��������� ������ �������� �����
	/// </summary>
	/// <param name="lParam">�������� ��������� ������� ������</param>
	void			�lickButton(LPARAM lParam);
};

