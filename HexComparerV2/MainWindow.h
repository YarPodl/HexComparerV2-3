#pragma once
#include "stdafx.h"
#include "DisplayArea.h"

/// <summary>
/// Класс, содержащий всю функциональность главного окна
/// </summary>
class MainWindow
{
public:

	/// <summary>
	/// Предварительная инициализация
	/// </summary>
	/// <param name="hInstance">Дескриптор приложения</param>
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
	/// <returns>Код результата</returns>
	static		LRESULT CALLBACK	StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	/// <summary>
	/// Дескриптор приложения
	/// </summary>
	HINSTANCE		m_hInst			= { 0 };

	/// <summary>
	/// Дескриптор главного окна
	/// </summary>
	HWND			m_hWnd			= { 0 };

	/// <summary>
	/// Объект, отвечающий за работу программы
	/// </summary>
	DisplayArea*	m_pDisplayArea	= NULL;

	/// <summary>
	/// Оконная процедура, вызываемая после определения конкретного объекта MainWindow
	/// </summary>
	/// <param name="hWnd">Дескриптор окна</param>
	/// <param name="message">Код сообщения</param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns>Возвращаемое значение зависит от типа сообщения</returns>
	virtual LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Прокрутка колесика мыши
	/// </summary>
	/// <param name="wParam">Значение прокрутки</param>
	void						MouseWheel(WPARAM wParam);

	/// <summary>
	/// Выбирает конкретную функцию скролла
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	void						Scroll(WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Регистрация класса окна
	/// </summary>
	/// <param name="hInstance">Дескриптор приложения</param>
	/// <returns>Код результата регистрации окна</returns>
	ATOM						RegisterMyClass(HINSTANCE hInstance);

	/// <summary>
	/// Создание главного окна
	/// </summary>
	/// <param name="hInstance">Дескриптор приложения</param>
	/// <returns>TRUE, если окно создано успешно</returns>
	BOOL						CreateMyWindow(HINSTANCE hInstance);
};
