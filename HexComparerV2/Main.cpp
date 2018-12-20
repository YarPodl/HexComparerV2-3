// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MainWindow.h"
             

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPWSTR    lpCmdLine,
			_In_ int       nCmdShow)
{
	MainWindow MyMainWindow;
	MyMainWindow.Create(hInstance);
	return MyMainWindow.Start(nCmdShow);
}