#pragma once
#include "stdafx.h"
#include "DisplayArea.h"

/// <summary>
/// �����, ���������� ��� ���������������� �������� ����
/// </summary>
class MainWindow
{
public:

	/// <summary>
	/// ��������������� �������������
	/// </summary>
	/// <param name="hInstance">���������� ����������</param>
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
	/// <returns>��� ����������</returns>
	static		LRESULT CALLBACK	StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	/// <summary>
	/// ���������� ����������
	/// </summary>
	HINSTANCE		m_hInst			= { 0 };

	/// <summary>
	/// ���������� �������� ����
	/// </summary>
	HWND			m_hWnd			= { 0 };

	/// <summary>
	/// ������, ���������� �� ������ ���������
	/// </summary>
	DisplayArea*	m_pDisplayArea	= NULL;

	/// <summary>
	/// ������� ���������, ���������� ����� ����������� ����������� ������� MainWindow
	/// </summary>
	/// <param name="hWnd">���������� ����</param>
	/// <param name="message">��� ���������</param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns>������������ �������� ������� �� ���� ���������</returns>
	virtual LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// ��������� �������� ����
	/// </summary>
	/// <param name="wParam">�������� ���������</param>
	void						MouseWheel(WPARAM wParam);

	/// <summary>
	/// �������� ���������� ������� �������
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	void						Scroll(WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// ����������� ������ ����
	/// </summary>
	/// <param name="hInstance">���������� ����������</param>
	/// <returns>��� ���������� ����������� ����</returns>
	ATOM						RegisterMyClass(HINSTANCE hInstance);

	/// <summary>
	/// �������� �������� ����
	/// </summary>
	/// <param name="hInstance">���������� ����������</param>
	/// <returns>TRUE, ���� ���� ������� �������</returns>
	BOOL						CreateMyWindow(HINSTANCE hInstance);
};
