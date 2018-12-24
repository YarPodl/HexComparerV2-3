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
	AreaOfFile		m_areasOfFiles[COUNT_OF_FILES];

	/// <summary>
	/// �������� �� �����
	/// </summary>
	FileCommander	m_fileCommander;

	/// <summary>
	/// ������� ����
	/// </summary>
	HWND			hWnd;
	 
	/// <summary>
	/// ������� ��������
	/// </summary>
	HINSTANCE		hInst;

	/// <summary>
	/// ����� ���������� �����
	/// </summary>
	INT64			m_countRows;

	/// <summary>
	/// ������� ���������� (����� ������ ������������ ������)
	/// </summary>
	INT64			scrollPos;
	
	/// <summary>
	/// ���������� �������������� �����
	/// </summary>
	INT64			m_scrollInc;
	 
	/// <summary>
	/// ������������ ���������� ���� ����� �������� ������
	/// </summary>
	INT64			countOfByte;
	 
	/// <summary>
	/// ������ ���������� ������� ����
	/// </summary>
	INT				heightClient;
	 
	/// <summary>
	/// ������ ���������� ������� ����
	/// </summary>
	INT				widthClient;
	 
	/// <summary>
	/// ����������� �������� ���������� ������� ����� ��� ���� ��������
	/// </summary>
	INT				m_minCountOfVisibleRows = 0;
	 
	/// <summary>
	/// ����������� ����� �������� �������� ���������� � �������� �������
	/// </summary>
	double			m_ratioOfScroll = 0;
	 
	/// <summary>
	/// ������� �����
	/// </summary>
	HFONT			hFont = NULL;
	 
	/// <summary>
	/// ������������ ������� �������
	/// </summary>
	INT				m_maxScrollPos;


	bool			OpenFileFromEdit();
	void			Scroll();
	void			SetSizeAreaOfFile();
public:
					DisplayArea(HWND hWnd, HINSTANCE hInst);
					~DisplayArea();
	void			ChangeSize(LPARAM lParam);
	LRESULT			Command(WPARAM wParam, LPARAM lParam);
	void			Paint(HDC hdc, PAINTSTRUCT &ps);
	void			scrollLineUp();
	void			scrollLineDown();
	void			scrollPageUp(LPARAM lParam);
	INT				getCountOfVisibleRows(LPARAM lParam);
	void			scrollPageDown(LPARAM lParam);
	void			scrollBegin();
	void			scrollEnd();
	void			scrollTo(LPARAM lParam);
	void			Scroll(WPARAM wParam, LPARAM lParam);
	bool			loadFile(INT indexFile, LPCWSTR fileName);
};

