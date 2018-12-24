#pragma once
#include "stdafx.h"
#include "DisplayArea.h"

class MainWindow
{
public:

	/// <summary>
	/// Предварительная инициализация
	/// </summary>
	/// <param name="hInstance">Дескриптор модуля</param>
	/// <returns>TRUE - если инициализация успешна</returns>
	virtual		BOOL				Create(HINSTANCE hInstance);

	/// <summary>
	/// Запускает главное окно программы
	/// </summary>
	/// <param name="nCmdShow">Cостояние показа окна</param>
	/// <returns>Код завершения работы</returns>
	virtual		INT					Start(INT nCmdShow);

	/// <summary>
	/// Оконная процедура для всех объектов класса
	/// </summary>
	/// <param name="hWnd">Дескриптор окна</param>
	/// <param name="message">Сообщение</param>
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
