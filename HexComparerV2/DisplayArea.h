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
	// �������� �������, ���������� �� ������� ������
	AreaOfFile		m_areasOfFiles[COUNT_OF_FILES];

	// �������� �� �����
	FileCommander	m_fileCommander;

	// ������� ����
	HWND			hWnd;

	// ������� ��������
	HINSTANCE		hInst;

	// ����� ���������� �����
	INT64			countRows;

	// ������� ���������� (����� ������ ������������ ������)
	INT64			scrollPos;

	// ���������� �������������� �����
	INT64			m_scrollInc;

	// ������������ ���������� ���� ����� �������� ������
	INT64			countOfByte;

	// ������ ���������� ������� ����
	int				heightClient;

	// ������ ���������� ������� ����
	int				widthClient;

	//int				m_minCountOfVisibleRows = 0;

	// ����������� ����� �������� �������� ���������� � �������� �������
	double			ratioOfScroll = 0;

	// ������� �����
	HFONT			hFont = NULL;

	// ������������ ������� �������
	unsigned int	maxScrollPos;


	void			�hangeEdit(LPARAM lParam);
	void			�lickButton(LPARAM lParam);
	bool			OpenFileDialog(LPWSTR file);
	void			Scroll();
	void			SetSizeAreaOfFile();
public:
					DisplayArea(HWND hWnd, HINSTANCE hInst);
					~DisplayArea();
	void			ChangeSize(LPARAM lParam);
	void			Command(WPARAM wParam, LPARAM lParam);
	void			Paint(HDC hdc, PAINTSTRUCT &ps);
	void			scrollLineUp();
	void			scrollLineDown();
	void			scrollPageUp(LPARAM lParam);
	int				getCountOfVisibleRows(LPARAM lParam);
	void			scrollPageDown(LPARAM lParam);
	void			scrollBegin();
	void			scrollEnd();
	void			scrollTo(LPARAM lParam);
	bool			loadFile(int indexFile, LPCWSTR fileName);
};

