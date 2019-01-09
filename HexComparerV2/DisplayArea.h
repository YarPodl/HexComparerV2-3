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
	/// Содержит общие данные о скролле
	/// </summary>
	DataOfScroll	m_DataOfScroll		= { 0 };
	 
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
	/// Текущий шрифт
	/// </summary>
	HFONT			m_hFont				= NULL;

	/// <summary>
	/// Кнопка поиска следующего отличия
	/// </summary>
	HWND			m_hButtonNext		= NULL;

	/// <summary>
	/// Кнопка поиска предыдущего отличия
	/// </summary>
	HWND			m_hButtonPrev		= NULL;

	/// <summary>
	/// Кнопка поиска количества отличий
	/// </summary>
	HWND			m_hButtonSearch		= NULL;

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

	void			NextDifference();
	void			PrevDifference();
	void			CountDifference();

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
	/// Возвращает количество видимых строк у области, которой принадлежит скроллбар
	/// </summary>
	/// <param name="lParam">Содержит дескриптор скроллбара (допустимо NULL)</param>
	INT				GetCountOfVisibleRows(LPARAM lParam);

	/// <summary>
	/// Вычисляет минимальное количество видимых строк для областей
	/// </summary>
	void			UpdateMinCountOfVisibleRows();

	/// <summary>
	/// Открывает файл по пути из edit, который находится в фокусе ввода
	/// </summary>
	/// <returns>Был ли открыт файл</returns>
	bool			OpenFileFromEdit();

	/// <summary>
	/// Закрывает файл для переданного edit
	/// </summary>
	/// <param name="hEdit">Содержит дескритор поля ввода, которое было изменено</param>
	void			СhangeEdit(HWND hEdit);

	/// <summary>
	/// Открывает диалог открытия файла
	/// </summary>
	/// <param name="hButton">Содержит дескритор нажатой кнопки</param>
	void			СlickButton(HWND hButton);
};

