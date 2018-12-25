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
	AreaOfFile		m_AreasOfFiles[COUNT_OF_FILES];

	/// <summary>
	/// Отвечает за файлы
	/// </summary>
	FileCommander	m_FileCommander;

	/// <summary>
	/// Дескриптор главного окна
	/// </summary>
	HWND			m_hWnd				= NULL;
	 
	/// <summary>
	/// Дескриптор приложения
	/// </summary>
	HINSTANCE		m_hInst				= NULL;

	/// <summary>
	/// Общее количество строк
	/// </summary>
	INT64			m_CountRows			= 0;

	/// <summary>
	/// Позиция скроллинга (номер первой отображаемой строки)
	/// </summary>
	INT64			m_ScrollPos			= 0;
	
	/// <summary>
	/// Количество прокручиваемых строк
	/// </summary>
	INT64			m_ScrollInc			= 0;
	 
	/// <summary>
	/// Максимальное количество байт среди открытых файлов
	/// </summary>
	INT64			m_CountOfByte		= 0;
	 
	/// <summary>
	/// Высота клиентской области окна
	/// </summary>
	INT				m_HeightClient		= 0;
	 
	/// <summary>
	/// Ширина клиентской области окна
	/// </summary>
	INT				m_WidthClient		= 0;
	 
	/// <summary>
	/// Минимальное значение количества видимых строк для всех областей
	/// </summary>
	INT				m_MinCountOfVisibleRows = 0;
	 
	/// <summary>
	/// Соотношение между реальной позицией скроллинга и позицией бегунка
	/// </summary>
	double			m_RatioOfScroll		= 0;
	 
	/// <summary>
	/// Текущий шрифт
	/// </summary>
	HFONT			m_hFont				= NULL;
	 
	/// <summary>
	/// Максимальная позиция бегунка
	/// </summary>
	INT				m_MaxScrollPos		= 0;

	/// <summary>
	/// Скроллинг, согласно значению m_ScrollInc
	/// </summary>
	void			Scroll();

	/// <summary>
	/// Установка новых размеров и параметров для всех областей
	/// </summary>
	void			SetSizeAreaOfFile();

	/// <summary>
	/// Вычисление всех параметров
	/// </summary>
	void			UpdateData();

public:
	/// <summary>
	/// Инициализация
	/// </summary>
	/// <param name="hWnd">Дескриптор окна</param>
	/// <param name="hInst">Дескриптор приложения</param>
					DisplayArea(HWND hWnd, HINSTANCE hInst);
	/// <summary>
	/// Закрывает все Handles
	/// </summary>
					~DisplayArea();

	/// <summary>
	/// Изменение размеров окна
	/// </summary>
	/// <param name="lParam">Содержит новые размеры клиентской области окна</param>
	void			ChangeSize(LPARAM lParam);

	/// <summary>
	/// Производит рисование всех областей
	/// </summary>
	/// <param name="hdc">Контекст устройства для рисования</param>
	/// <param name="ps">Структура, содержащая сведения о рисовании</param>
	void			Paint(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// Скролл вверх на одну строку
	/// </summary>
	void			ScrollLineUp();

	/// <summary>
	/// Скролл вниз на одну строку
	/// </summary>
	void			ScrollLineDown();

	/// <summary>
	/// Скролл вверх на одну страницу
	/// </summary>
	/// <param name="lParam">Содержит дескриптор скроллбара (допустимо NULL)</param>
	void			ScrollPageUp(LPARAM lParam);

	/// <summary>
	/// Скролл вниз на одну страницу
	/// </summary>
	/// <param name="lParam">Содержит дескриптор скроллбара (допустимо NULL)</param>
	void			ScrollPageDown(LPARAM lParam);


	/// <summary>
	/// Скролл в начало
	/// </summary>
	void			ScrollBegin();

	/// <summary>
	/// Скролл в конец
	/// </summary>
	void			ScrollEnd();

	/// <summary>
	/// Скролл бегунком в конкретную позицию
	/// </summary>
	/// <param name="lParam">Содержит дескриптор скроллбара</param>
	void			ScrollTo(LPARAM lParam);
	
	/// <summary>
	/// Возвращает количество видимых строк
	/// </summary>
	/// <param name="lParam">Содержит дескриптор скроллбара (допустимо NULL)</param>
	INT				GetCountOfVisibleRows(LPARAM lParam);

	/// <summary>
	/// Открывает файл по пути из edit, который находится в фокусе ввода
	/// </summary>
	/// <returns>Был ли открыт файл</returns>
	bool			OpenFileFromEdit();

	/// <summary>
	/// Закрывает файл для переданного edit
	/// </summary>
	/// <param name="lParam">Содержит дескритор поля ввода, которое было изменено</param>
	void			СhangeEdit(LPARAM lParam);

	/// <summary>
	/// Открывает диалог открытия файла
	/// </summary>
	/// <param name="lParam">Содержит дескритор нажатой кнопки</param>
	void			СlickButton(LPARAM lParam);
};

