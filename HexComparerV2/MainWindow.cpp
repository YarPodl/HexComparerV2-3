#include "stdafx.h"
#include "MainWindow.h"

#define ACCELERATOR_COMMAND 1

BOOL MainWindow::Create(HINSTANCE hInstance)
{
	m_hInst = hInstance;

	// Регистрация класса окна
	RegisterMyClass(hInstance);

	// Проверка успешности
	if (!CreateMyWindow(hInstance))
		return FALSE; 

	m_pDisplayArea = new DisplayArea(m_hWnd, m_hInst);

	return TRUE;
}

INT MainWindow::Start(INT nCmdShow)
{
	// Отображение окна
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	// Загрузка таблицы акселераторов
	HACCEL	hAccelTabel = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	MSG		Msg;	// Сообщение

	// Главный цикл сообщений
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(m_hWnd, hAccelTabel, & Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	return (INT)Msg.wParam;
}

LRESULT MainWindow::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT		Result;	// Результата

	// Если окно создается
	if (message == WM_NCCREATE)
	{
		// Получение структуры с информацией
		MDICREATESTRUCT * pMDIC = (MDICREATESTRUCT *)((LPCREATESTRUCT)lParam)->lpCreateParams;

		// Получение из структуры адреса объекта MainWindow, который создал окно
		MainWindow * mainWindow = (MainWindow *)(pMDIC->lParam);

		// Сохранение адреса объекта MainWindow
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)mainWindow);
	}

	// Получение адреса объекта MainWindow, который создал окно
	MainWindow *mainWindow = (MainWindow *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	
	// Если получить не удалось
	if (!mainWindow)
		return DefWindowProcW(hWnd, message, wParam, lParam);

	// Вызов оконной процедуры конкретного объекта
	Result = mainWindow->WndProc(hWnd, message, wParam, lParam);

	return Result;
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MINMAXINFO*	pMinMaxInfo		= NULL;		// Минимальные размеры окна

	switch (message)
	{
	// Изменение размера окна
	case WM_SIZE:
		m_pDisplayArea->ChangeSize(lParam);
		return 0;

	// Сообщение с кодом команды
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		// Нажатие кнопки
		case BN_CLICKED:
			m_pDisplayArea->СlickButton((HWND)lParam);
			break;

		// Изменение поля
		case EN_CHANGE:
			m_pDisplayArea->СhangeEdit((HWND)lParam);
			break;

		// Обработка клавиш акселераторов
		case ACCELERATOR_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_BEGIN:
				m_pDisplayArea->ScrollBegin();
				break;

			case ID_END:
				m_pDisplayArea->ScrollEnd();
				break;

			case ID_PAGEUP:
				m_pDisplayArea->ScrollPageUp(NULL);
				break;

			case ID_PAGEDOWN:
				m_pDisplayArea->ScrollPageDown(NULL);
				break;

			case ID_LINEUP:
				m_pDisplayArea->ScrollLineUp();
				break;

			case ID_LINEDOWN:
				m_pDisplayArea->ScrollLineDown();
				break;

			case ID_ENTER:
				m_pDisplayArea->OpenFileFromEdit();
				break;

			default:
				break;
			}
		default:
			break;
		}

		return 0;

	// Перерисовка
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		m_pDisplayArea->Paint(hdc, ps);
		EndPaint(hWnd, &ps);
	}
	break;

	// Скролл
	case WM_VSCROLL:
		Scroll(wParam, lParam);
		return 0;

	// Прокрутка колесика мыши
	case WM_MOUSEWHEEL:
		MouseWheel(wParam);
		break;

	// Ответ на запрос о минимальных размерах окна
	case WM_GETMINMAXINFO:
		pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = MIN_WIDTH_WINDOW;
		pMinMaxInfo->ptMinTrackSize.y = MIN_HEIGHT_WINDOW;
		break;

	// Закрытие окна
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


void MainWindow::MouseWheel(WPARAM wParam)
{
	INT WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// Прокрутка колесика мыши

	// Прокрутка вверх
	for (; WheelDelta > 0; WheelDelta -= WHEEL_DELTA)
		m_pDisplayArea->ScrollLineUp();

	// Прокрутка вниз
	for (; WheelDelta < 0; WheelDelta += WHEEL_DELTA)
		m_pDisplayArea->ScrollLineDown();
}


void MainWindow::Scroll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{

	case SB_BOTTOM:
		m_pDisplayArea->ScrollEnd();
		break;

	case SB_TOP:
		m_pDisplayArea->ScrollBegin();
		break;

	case SB_LINEUP:
		m_pDisplayArea->ScrollLineUp();
		break;

	case SB_LINEDOWN:
		m_pDisplayArea->ScrollLineDown();
		break;

	case SB_PAGEUP:
		m_pDisplayArea->ScrollPageUp(lParam);
		break;

	case SB_PAGEDOWN:
		m_pDisplayArea->ScrollPageDown(lParam);
		break;

	case SB_THUMBTRACK:
		m_pDisplayArea->ScrollTo(lParam);
		break;

	case SB_THUMBPOSITION:
		m_pDisplayArea->ScrollTo(lParam);
		break;

	default:
		break;
	}
}


ATOM MainWindow::RegisterMyClass(HINSTANCE hInstance)
{
	WNDCLASSEXW Wcex;	// Данные о классе окна

	Wcex.cbSize			= sizeof(WNDCLASSEX);

	Wcex.style			= CS_HREDRAW | CS_VREDRAW;
	Wcex.lpfnWndProc	= StaticWndProc;
	Wcex.cbClsExtra		= 0;
	Wcex.cbWndExtra		= 0;
	Wcex.hInstance		= hInstance;
	Wcex.hIcon			= LoadIcon(hInstance, NULL);
	Wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	Wcex.hbrBackground	= BACKGROUND_WINDOW;
	Wcex.lpszMenuName	= NULL;
	Wcex.lpszClassName	= WINDOWCLASS;
	Wcex.hIconSm		= LoadIcon(hInstance, NULL);

	return RegisterClassExW(&Wcex);
}

BOOL MainWindow::CreateMyWindow(HINSTANCE hInstance)
{
	// Создание структуры, содержащей адрес объекта MainWindow
	MDICREATESTRUCT MdiStruct;

	memset(&MdiStruct, 0, sizeof(MdiStruct));
	
	MdiStruct.lParam = (LPARAM)this;

	// Создание окна
	m_hWnd = CreateWindowW(
		WINDOWCLASS,
		(LPCWSTR)TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		WIDTH_WINDOW, HEIGHT_WINDOW,
		NULL, NULL, 
		hInstance, 
		& MdiStruct);

	return !!m_hWnd;	
}
