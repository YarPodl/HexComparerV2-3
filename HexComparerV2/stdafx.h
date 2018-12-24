#pragma once



#include <windows.h>
#include "resource.h"

// Максимальная длина строк (буферных) в программе
#define MAX_SIZE_STRING 100

// Максимальная длина строк (буферных) в программе
#define LENGTH_PATH 1024

// Высота символа
#define MAX_FONT_SIZE_HEIGHT 15

// Отношение ширины символа к его высоте
#define FONT_SIZE_RELATION 0.45

// Ширина символа
//#define FONT_SIZE_WIDTH 6

// Шрифт
#define FONT_FAMILY L"Lucida Console"
//#define FONT_FAMILY L"Times new Roman Cyr"
//#define FONT_FAMILY L"Courier new"
//#define FONT_FAMILY L"Consolas"
//#define FONT_FAMILY L"Symbol"



// Расстояние между строк (в пикселях)
#define INTERLACED_SPACE 2;

// Количество байтов, отображаемых на одной строке
#define LENGTH_OF_BYTE_STRINGS 8

// Количество сравниваемых файлов
#define COUNT_OF_FILES 3

// Начальная высота окна
#define HEIGHT_WINDOW 600

#define BACKGROUND_WINDOW (HBRUSH)(COLOR_WINDOW + 1)

// Начальная ширина окна
#define WIDTH_WINDOW 1050

// Минимальная высота окна
#define MIN_HEIGHT_WINDOW 150

// Минимальная ширина окна
#define MIN_WIDTH_WINDOW 400

// Высота меню
#define HEIGHT_MENU 20

// Высота кнопок
#define HEIGHT_BUTTONS 20

// Ширина кнопок
#define WIDTH_BUTTONS 30

// Цвет различающихся байтов
#define TEXT_COLOR_SELECT RGB(255, 0, 0)

// Отступ сверху
#define INDENT_OF_TOP 3

// Отступы перед столбцом с номером x (ширина в символах)
#define INDENT1 1	
#define INDENT2 4
#define INDENT3 4
#define INDENT4 2

// Количество символов для отображения байта
#define CharsForByte 2

// Отступы между байтами 2 столбца (в символах)
#define INDENT_BETWEEN_BYTES2 1

// Ширина scrollBar
#define WIDTH_SCROLLBAR 20

// Отступы между буквами 3 столбца (в символах)
#define INDENT_BETWEEN_LETTERS3 0

// Заголовок окна программы
#define TITLE "Сравнение файлов"

// Класс окна программы
#define WINDOWCLASS L"HexComparerV2"