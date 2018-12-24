#pragma once
#include "stdafx.h"
#include "DisplayArea.h"

class MainWindow
{
public:

	virtual		BOOL				Create(HINSTANCE hInstance);
	virtual		INT					Start(INT nCmdShow);
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
