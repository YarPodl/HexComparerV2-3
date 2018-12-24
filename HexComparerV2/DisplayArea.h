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
	/// <summary>
	/// Содержит объекты, отвечающие за области файлов
	/// </summary>
	AreaOfFile		m_areasOfFiles[COUNT_OF_FILES];

	/// <summary>
	/// Отвечает за файлы
	/// </summary>
	FileCommander	m_fileCommander;

	/// <summary>
	/// Главное окно
	/// </summary>
	HWND			hWnd;
	 
	/// <summary>
	/// Главный контекст
	/// </summary>
	HINSTANCE		hInst;

	/// <summary>
	/// Общее количество строк
	/// </summary>
	INT64			m_countRows;

	/// <summary>
	/// Позиция скроллинга (номер первой отображаемой строки)
	/// </summary>
	INT64			scrollPos;
	
	/// <summary>
	/// Количество прокручиваемых строк
	/// </summary>
	INT64			m_scrollInc;
	 
	/// <summary>
	/// Максимальное количество байт среди открытых файлов
	/// </summary>
	INT64			countOfByte;
	 
	/// <summary>
	/// Высота клиентской области окна
	/// </summary>
	INT				heightClient;
	 
	/// <summary>
	/// Ширина клиентской области окна
	/// </summary>
	INT				widthClient;
	 
	/// <summary>
	/// Минимальное значение количества видимых строк для всех областей
	/// </summary>
	INT				m_minCountOfVisibleRows = 0;
	 
	/// <summary>
	/// Соотношение между реальной позицией скроллинга и позицией бегунка
	/// </summary>
	double			m_ratioOfScroll = 0;
	 
	/// <summary>
	/// Текущий шрифт
	/// </summary>
	HFONT			hFont = NULL;
	 
	/// <summary>
	/// Максимальная позиция бегунка
	/// </summary>
	INT				m_maxScrollPos;


	bool			OpenFileFromEdit();
	void			Scroll();
	void			SetSizeAreaOfFile();
public:
					DisplayArea(HWND hWnd, HINSTANCE hInst);
					~DisplayArea();
	void			ChangeSize(LPARAM lParam);
	LRESULT			Command(WPARAM wParam, LPARAM lParam);
	void			Paint(HDC hdc, PAINTSTRUCT &ps);
	void			scrollLineUp();
	void			scrollLineDown();
	void			scrollPageUp(LPARAM lParam);
	INT				getCountOfVisibleRows(LPARAM lParam);
	void			scrollPageDown(LPARAM lParam);
	void			scrollBegin();
	void			scrollEnd();
	void			scrollTo(LPARAM lParam);
	void			Scroll(WPARAM wParam, LPARAM lParam);
	bool			loadFile(INT indexFile, LPCWSTR fileName);
};

