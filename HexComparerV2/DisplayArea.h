#pragma once
#include "stdafx.h"

#include "AreaOfFile.h"
#include "FileCommander.h"

/// <summary>
/// Координирует работу всех элементов клиентской области
/// </summary>
class DisplayArea
{
private:
	// Содержит объекты, отвечающие за области файлов
	AreaOfFile		m_areasOfFiles[COUNT_OF_FILES];

	// Отвечает за файлы
	FileCommander	m_fileCommander;

	// Главное окно
	HWND			hWnd;

	// Главный контекст
	HINSTANCE		hInst;

	// Общее количество строк
	INT64			m_countRows;

	// Позиция скроллинга (номер первой отображаемой строки)
	INT64			scrollPos;

	// Количество прокручиваемых строк
	INT64			m_scrollInc;

	// Максимальное количество байт среди открытых файлов
	INT64			countOfByte;

	// Высота клиентской области окна
	INT				heightClient;

	// Ширина клиентской области окна
	INT				widthClient;

	// Минимальное значение количества видимых строк для всех областей
	INT				m_minCountOfVisibleRows = 0;

	// Соотношение между реальной позицией скроллинга и позицией бегунка
	double			m_ratioOfScroll = 0;

	// Текущий шрифт
	HFONT			hFont = NULL;

	// Максимальная позиция бегунка
	INT				m_maxScrollPos;


	void			СhangeEdit(LPARAM lParam);
	void			СlickButton(LPARAM lParam);
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
	INT				getCountOfVisibleRows(LPARAM lParam);
	void			scrollPageDown(LPARAM lParam);
	void			scrollBegin();
	void			scrollEnd();
	void			scrollTo(LPARAM lParam);
	bool			loadFile(INT indexFile, LPCWSTR fileName);
};

