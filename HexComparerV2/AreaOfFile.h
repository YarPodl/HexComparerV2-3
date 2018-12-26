#pragma once
#include "stdafx.h"
#include "FileMapping.h"
#include "FileCommander.h"

#define LENGTH_OF_BUFFER 18	// Длина буфера
#define LENGTH_OF_BYTE 3	// Длина буфера, содержащего байты в виде шестнадцатеричного числа

/// <summary>
/// Содержит общие данные о скролле
/// </summary>
struct DataOfScroll
{
	/// <summary>
	/// Позиция скроллинга (номер первой отображаемой строки)
	/// </summary>
	INT64			ScrollPos = 0;

	/// <summary>
	/// Соотношение между реальной позицией скроллинга и позицией бегунка
	/// </summary>
	DOUBLE			RatioOfScroll = 0;

	/// <summary>
	/// Максимальная позиция бегунка
	/// </summary>
	INT				MaxScrollPos = 0;

	/// <summary>
	/// Количество прокручиваемых строк
	/// </summary>
	INT64			ScrollInc = 0;

	/// <summary>
	/// Максимальное число строк (реальных позиций скролла)
	/// </summary>
	INT64			CountRows = 0;
};


/// <summary>
/// Класс, отвечающий за область отображения одного файла
/// </summary>
class AreaOfFile
{
private:
	/// <summary>
	/// Собственный номер текущей области
	/// </summary>
	INT				m_NumberOfArea					= -1;

	/// <summary>
	/// Указатель на объект, содержащий файлы
	/// </summary>
	FileCommander * m_pFileCommander				= NULL;

	/// <summary>
	/// Содержит общие данные о скролле
	/// </summary>
	DataOfScroll * m_pDataOfScroll					= NULL;

	/// <summary>
	/// Дескриптор главного окна
	/// </summary>
	HWND			m_hWnd							= NULL;

	/// <summary>
	/// Дескриптор приложения
	/// </summary>
	HINSTANCE		m_hInst							= NULL;

	/// <summary>
	/// Координаты области с данными файла
	/// </summary>
	RECT			m_RectData						= { 0 };

	/// <summary>
	/// Координаты области с меню
	/// </summary>
	RECT			m_RectMenu						= { 0 };

	/// <summary>
	/// Строка формата для номера строки
	/// </summary>
	WCHAR			m_Format[LENGTH_OF_BUFFER]		= { 0 };

	/// <summary>
	/// Дескриптор поля ввода
	/// </summary>
	HWND			m_hEdit							= NULL;

	/// <summary>
	/// Дескриптор кнопки
	/// </summary>
	HWND			m_hButton						= NULL;

	/// <summary>
	/// Дескриптор скролл бара
	/// </summary>
	HWND			m_hScrollBar					= NULL;

	/// <summary>
	/// Дескриптор шрифта
	/// </summary>
	HFONT			m_hFont							= NULL;

	/// <summary>
	/// Координата x для столбца с байтами
	/// </summary>
	INT				m_IndentForBytes				= 0;

	/// <summary>
	/// Координата x для столбца представлением байтов как символов
	/// </summary>
	INT				m_IndentForLetters				= 0;
	
	/// <summary>
	/// Ширина символа
	/// </summary>
	INT				m_WidthChar						= 0;

	/// <summary>
	/// Высота символа
	/// </summary>
	INT				m_HeightChar					= 0;

	/// <summary>
	/// Длина номера строки
	/// </summary>
	INT				m_LengthOfNumberRow				= 0;

	/// <summary>
	/// Количество видимых строк
	/// </summary>
	INT				m_CountOfVisibleRows			= 0;

	/// <summary>
	/// Обновляет данные скролла
	/// </summary>
	void			UpdateScrollInfo();

	/// <summary>
	/// Рисует номер строки
	/// </summary>
	/// <param name="hdc">Дескриптор контекста устройства</param>
	/// <param name="numberLine">Номер строки на экране</param>
	/// <param name="numberLineForView">Номер, который нужно отобразить</param>
	void inline		PaintNumberLine(HDC hdc, INT numberLine, INT64 numberLineForView);

	/// <summary>
	/// Рисует один байт
	/// </summary>
	/// <param name="hdc">Дескриптор контекста устройства</param>
	/// <param name="numberLine">Номер строки на экране</param>
	/// <param name="numberByte">Номер байта в строке</param>
	/// <param name="stringOfByte">Строка, содержащая шестнадцатеричное представление</param>
	/// <param name="charOfByte">Символ байта</param>
	void inline		PaintByte(HDC hdc, INT numberLine, INT numberByte, WCHAR stringOfByte[], CHAR charOfByte);

	/// <summary>
	/// Формирует шестнадцатеричное представление для байта
	/// </summary>
	/// <param name="in">Значение байта</param>
	/// <param name="out">Шестнадцатеричное представление</param>
	void inline		ByteToHexString(byte in, OUT WCHAR out[]);

	/// <summary>
	/// Обновляет шрифт
	/// </summary>
	void			UpdateFont();

	/// <summary>
	/// Обновляет данные о номере строки
	/// </summary>
	void			UpdateNumberOfRow();

	/// <summary>
	/// Открывает стандартный диалог открытия файла
	/// </summary>
	/// <param name="file">Буфер для имени файла</param>
	/// <returns></returns>
	BOOL			OpenFileDialog(LPWSTR file);

	/// <summary>
	/// Рисует рамку вокруг области
	/// </summary>
	/// <param name="hdc">Дескриптор контекста устройства</param>
	void			PaintBorder(HDC hdc);


public:
	
	/// <summary>
	/// Инициализирует объект
	/// </summary>
	/// <param name="number">Номер области</param>
	/// <param name="hWnd">Дескриптор главного окна</param>
	/// <param name="hInst">Дескриптор приложения</param>
	/// <param name="hFont">Дескриптор шрифта (применяется для edit)</param>
	/// <param name="fileCommander">Указатель на объект, содержащий файлы</param>
	/// <param name="m_DateOfScroll">Указатель на объект, содержащий данные о скролле, общие для всех областей</param>
	/// <returns>Успешность инициализации</returns>
	BOOL		Initialize(INT number, HWND hWnd, HINSTANCE hInst, 
					HFONT hFont, 
					FileCommander * fileCommander, 
					DataOfScroll * m_DateOfScroll);

	/// <summary>
	/// Закрытие всех открытых дескрипторов
	/// </summary>
	void		CloseHandle();

	/// <summary>
	/// Рисование всей области
	/// </summary>
	/// <param name="hdc">Контекст устройства для рисования</param>
	/// <param name="ps">Структура, содержащая сведения о рисовании</param>
	void		PaintArea(HDC hdc, PAINTSTRUCT &ps);

	/// <summary>
	/// Устанавливает новый размер области
	/// </summary>
	/// <param name="client">Содержит новый размер области</param>
	void		setSize(RECT client);

	/// <summary>
	/// Возвращает дескриптор кнопки
	/// </summary>
	/// <returns>Дескриптор кнопки</returns>
	HWND		GetButton();

	/// <summary>
	/// Возвращает дескриптор поля ввода
	/// </summary>
	/// <returns>Дескриптор поля ввода</returns>
	HWND		GetEdit();

	/// <summary>
	/// Возвращает дескриптор скролл бара
	/// </summary>
	/// <returns>Дескриптор скролл бара</returns>
	HWND		GetScrollBar();

	/// <summary>
	/// Возвращает число видимых строк
	/// </summary>
	/// <returns>Число видимых строк</returns>
	INT			GetCountOfVisibleRows();

	/// <summary>
	/// Устанавливает ряд данных
	/// </summary>
	void		UpdateData();

	/// <summary>
	/// Выполняет скроллинг содержимого области
	/// </summary>
	void		Scroll();

	/// <summary>
	/// Закрывает файл, если hEdit принадлежит области
	/// </summary>
	/// <param name="hEdit">Дескриптор окна</param>
	void		СhangeEdit(HWND hEdit);

	/// <summary>
	/// Открывает файл, если hButton принадлежит области
	/// </summary>
	/// <param name="hButton"></param>
	void		СlickButton(HWND hButton);

	/// <summary>
	/// Открытие файла по пути из edit
	/// </summary>
	void		OpenFile();
};

