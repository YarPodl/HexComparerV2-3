
#include "stdafx.h"
#include "MainWindow.h"
             

/// <summary>
/// Точка входа приложения
/// </summary>
INT APIENTRY wWinMain(_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPWSTR    lpCmdLine,
			_In_ INT       nCmdShow)
{
	MainWindow MyMainWindow;

	MyMainWindow.Create(hInstance);

	return MyMainWindow.Start(nCmdShow);
}