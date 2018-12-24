#pragma once
#include "stdafx.h"
#include "DisplayArea.h"

class MainWindow
{
public:

	/// <summary>
	/// ��������������� �������������
	/// </summary>
	/// <param name="hInstance">���������� ������</param>
	/// <returns>TRUE - ���� ������������� �������</returns>
	virtual		BOOL				Create(HINSTANCE hInstance);

	/// <summary>
	/// ��������� ������� ���� ���������
	/// </summary>
	/// <param name="nCmdShow">C�������� ������ ����</param>
	/// <returns>��� ���������� ������</returns>
	virtual		INT					Start(INT nCmdShow);

	/// <summary>
	/// ������� ��������� ��� ���� �������� ������
	/// </summary>
	/// <param name="hWnd">���������� ����</param>
	/// <param name="message">���������</param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	static		LRESULT CALLBACK	StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE		m_hInst			= { 0 };
	HWND			m_hWnd			= { 0 };
	DisplayArea*	m_displayArea	= NULL;

protected:

	virtual LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ATOM RegisterMyClass(HINSTANCE hInstance);

	BOOL CreateMyWindow(HINSTANCE hInstance);
};
