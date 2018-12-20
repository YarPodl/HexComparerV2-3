#include "stdafx.h"
#include "MainWindow.h"

// Èìÿ ñâîéñòâà, ñîäåðæàùåãî îáúåêò êëàññà
#define THIS_PROP L"this"


BOOL MainWindow::Create(HINSTANCE hInstance)
{
	m_hInst = hInstance;

	RegisterMyClass(hInstance);

	// Perform application initialization:
	if (!CreateMyWindow(hInstance))
	{
		return FALSE;
	}

	m_displayArea = new DisplayArea(m_hWnd, m_hInst);

	// Âðåìåííî
	if (!m_displayArea->loadFile(0, L"C:\\Users\\user\\Documents\\test"))
	{
		MessageBoxW(m_hWnd, L"ÎØÈÁÊÀ ÎÒÊÐÛÒÈß ÔÀÉËÀ dotnetfx35", L"", 0);
	}
	if (!m_displayArea->loadFile(1, L"C:\\Users\\user\\Documents\\dotnetfx351.exe"))
	{
		MessageBoxW(m_hWnd, L"ÎØÈÁÊÀ ÎÒÊÐÛÒÈß ÔÀÉËÀ dotnetfx351", L"", 0);
	}

	SetPropW(m_hWnd, THIS_PROP, this);

	return TRUE;
}

int MainWindow::Start(int nCmdShow)
{
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	HACCEL hAccelTabel = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		/*if (!TranslateAccelerator(hWnd, hAccelTabel, msg))
		{

		}*/
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT MainWindow::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	MainWindow *mainWindow = (MainWindow *)GetPropW(hWnd, THIS_PROP);
	if (!mainWindow)
	{
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	lResult = mainWindow->WndProc(hWnd, message, wParam, lParam);

	if (message == WM_DESTROY)
	{
		RemovePropW(hWnd, THIS_PROP);
	}

	return lResult;
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MINMAXINFO*	pMinMaxInfo		= NULL;

	m_wheelDelta = 0;

	switch (message)
	{
	case WM_SIZE:

		// 
		m_displayArea->ChangeSize(lParam);
		return 0;

	case WM_COMMAND:

		//
		m_displayArea->Command(wParam, lParam);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		m_displayArea->Paint(hdc, ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_BOTTOM:
			m_displayArea->scrollEnd();
			break;
		case SB_TOP:
			m_displayArea->scrollBegin();
			break;
		case SB_LINEUP:
			m_displayArea->scrollLineUp();
			break;

		case SB_LINEDOWN:
			m_displayArea->scrollLineDown();
			break;

		case SB_PAGEUP:
			m_displayArea->scrollPageUp(lParam);
			break;

		case SB_PAGEDOWN:
			m_displayArea->scrollPageDown(lParam);
			break;

		case SB_THUMBTRACK:
			m_displayArea->scrollTo(lParam);
			break;

		case SB_THUMBPOSITION:
			m_displayArea->scrollTo(lParam);
			break;

		default:
			break;
		}

		return 0;
	case WM_MOUSEWHEEL:
		m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
		for (; m_wheelDelta > WHEEL_DELTA; m_wheelDelta -= WHEEL_DELTA)
			m_displayArea->scrollLineUp();
		for (; m_wheelDelta < 0; m_wheelDelta += WHEEL_DELTA)
			m_displayArea->scrollLineDown();
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_END:
			m_displayArea->scrollEnd();
			break;
		case VK_HOME:
			m_displayArea->scrollBegin();
			break;
		case VK_UP:
			m_displayArea->scrollLineUp();
			break;

		case VK_DOWN:
			m_displayArea->scrollLineDown();
			break;

		case VK_PRIOR:
			m_displayArea->scrollPageUp(TODO);
			break;

		case VK_NEXT:
			m_displayArea->scrollPageDown();
			break;

			/*case VK_RETURN:
			m_displayArea->scrollPageDown();
			break;*/

		default:
			break;
		}
		return 0;
	case WM_GETMINMAXINFO:
		pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = MIN_WIDTH_WINDOW;
		pMinMaxInfo->ptMinTrackSize.y = MIN_HEIGHT_WINDOW;
		break;
		/*case WM_ERASEBKGND:
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM MainWindow::RegisterMyClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, NULL);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOWCLASS;
	wcex.hIconSm = LoadIcon(hInstance, NULL);

	return RegisterClassExW(&wcex);
}

BOOL MainWindow::CreateMyWindow(HINSTANCE hInstance)
{
	m_hWnd = CreateWindowW(
		WINDOWCLASS,
		(LPCWSTR)TITLE,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, 0,
		MIN_WIDTH_WINDOW, MIN_HEIGHT_WINDOW,
		nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	return TRUE;
}
